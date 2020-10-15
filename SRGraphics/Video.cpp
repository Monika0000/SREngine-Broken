#include "pch.h"
#include "Video.h"
extern "C" {
#include "libavcodec/avcodec.h"
#include "libavutil/imgutils.h"
#include "libswscale/swscale.h"
#include "libavformat/avformat.h"
#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"
}

#include "Texture.h"

static std::string av_error(const int errnum) {
	char errbuf[AV_ERROR_MAX_STRING_SIZE + 1];
	av_strerror(errnum, errbuf, AV_ERROR_MAX_STRING_SIZE);
	errbuf[AV_ERROR_MAX_STRING_SIZE] = '\0';
	return std::string(errbuf);
}

AVFrame* SpaRcle::Graphics::Video::createFrame(AVPacket* packet) {
	return nullptr;
}


bool SpaRcle::Graphics::Video::Load() {
	// Пробуем открыть видео файл
	if (avformat_open_input(&this->m_pFormatCtx, this->m_file_name.c_str(), NULL, NULL) != 0)
		return -1; // Не могу открыть файл

	// Пробуем получить информацию о потоке
	if (avformat_find_stream_info(m_pFormatCtx, NULL) < 0)
		return -1;

	// Получаем подробную информацию о файле: продолжительность, битрейд, контейнер и прочее
	av_dump_format(m_pFormatCtx, 0, this->m_file_name.c_str(), 0);
	
	//!======================================================================================
	// Находим первый кард
	m_video_stream = -1;
	for (int i = 0; i < m_pFormatCtx->nb_streams; i++)
		if (m_pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
			m_video_stream = i;
			break;
		}
	if (m_video_stream == -1)
		return -1; // Не нашли

	// Указатель куда будут сохраняться данные 
	AVCodecParameters* pCodecParams = m_pFormatCtx->streams[m_video_stream]->codecpar;
	// Находим подходящий декодер для файла
 	AVCodec* m_pCodec = avcodec_find_decoder(pCodecParams->codec_id);
	if (m_pCodec == NULL) {
		Debug::Error("Video::Load() : Unsupported codec!");
		return false; // Декодер не найден
	}

	// Копируем контекст
	this->m_pCodecCtx = avcodec_alloc_context3(m_pCodec);
	if (avcodec_parameters_to_context(m_pCodecCtx, pCodecParams) != 0) {
		Debug::Error("Video::Load() : failed avcodec_parameters_to_context");
		return false; // Ошибка копирования
	}

	// Открываем кодек
	if (avcodec_open2(m_pCodecCtx, m_pCodec, NULL) < 0)
		return false; // Не смогли открыть кодек
	//!======================================================================================

	// Здесь будет храниться кадр
	m_pFrame = av_frame_alloc();

	// Здесь храниться кадр преобразованный в RGB
	m_pFrameRGB = av_frame_alloc();
	if (m_pFrameRGB == NULL)
		return false;

	// Определяем необходимый размер буфера и выделяем память
	int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24, m_pCodecCtx->width,
		m_pCodecCtx->height, 1);
	m_buffer = (uint8_t*)av_malloc(numBytes * sizeof(uint8_t));
	//avpicture_fill((AVPicture*)m_pFrameRGB, m_buffer, AV_PIX_FMT_RGB24,
	//	m_pCodecCtx->width, m_pCodecCtx->height);

	//  Связуем кадр с вновь выделенным буфером.
	int size = av_image_fill_arrays(m_pFrame->data,	// destination
		m_pFrame->linesize,					// destination
		m_buffer,							// source
		AV_PIX_FMT_RGB24,					// source
		m_pCodecCtx->width,					// source & alingment
		m_pCodecCtx->height, 1
	);
	if (size < 0) {
		Debug::Error("Video::Load() : failed av_image_fill_arrays. Code : "+std::to_string(size));
		return false;
	}

	//!======================================================================================

	// Инициализируем SWS context для программного преобразования полученного кадра в RGB
	m_sws_ctx = sws_getContext(m_pCodecCtx->width,
		m_pCodecCtx->height,
		m_pCodecCtx->pix_fmt,
		m_pCodecCtx->width,
		m_pCodecCtx->height,
		AV_PIX_FMT_RGB24,
		SWS_BILINEAR,
		NULL,
		NULL,
		NULL
	);
	//!======================================================================================
	AVPacket* packet = av_packet_alloc();

	while (av_read_frame(m_pFormatCtx, packet) >= 0) {
		if (packet->stream_index == m_video_stream) {
			int sendPacketResult = avcodec_send_packet(m_pCodecCtx, packet);
			if (sendPacketResult == AVERROR(EAGAIN)) {
				// Decoder can't take packets right now. Make sure you are draining it.
				break;
			}
			else if (sendPacketResult < 0) {
				// Failed to send the packet to the decoder
				break;
			}

			//!=================================

			// Get decoded frame from decoder
			int decodeFrame = avcodec_receive_frame(m_pCodecCtx, m_pFrame);
			if (decodeFrame == AVERROR(EAGAIN)) {
				// The decoder doesn't have enough data to produce a frame
				// Not an error unless we reached the end of the stream
				// Just pass more packets until it has enough to produce a frame
			}
			else if (decodeFrame < 0) {
				// Failed to get a frame from the decoder
			}
			else {
				//std::cout << m_pFrame->width << std::endl;
			}
		}
		av_packet_unref(packet);
	}
	av_free(m_buffer);

	av_free(m_pFrame);

	// Close the codec
	avcodec_close(this->m_pCodecCtx);

	// Close the video file
	avformat_close_input(&this->m_pFormatCtx);

	return true;
}

/*
m_format_context = avformat_alloc_context();

int err = avformat_open_input(&m_format_context, m_file_name.c_str(), NULL, NULL);
if (err < 0) {
	Debug::Error("Video::Load() : ffmpeg: Unable to open input file");
	return false;
}

// Retrieve stream information
err = avformat_find_stream_info(m_format_context, NULL);
if (err < 0) {
	Debug::Error("Video::Load() : ffmpeg: Unable to find stream info");
	return false;
}

// Find the first video stream
for (this->m_video_stream = 0; m_video_stream < m_format_context->nb_streams; ++m_video_stream) {
	if (m_format_context->streams[m_video_stream]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
		break;
	}
}

if (m_video_stream == m_format_context->nb_streams) {
	Debug::Error("Video::Load() : ffmpeg: Unable to find video stream");
	return false;
}

m_pCodecCtx = avcodec_alloc_context3(NULL);

AVCodecParameters* m_codec_parametrs = m_format_context->streams[m_video_stream]->codecpar;
//AVCodec* codec = avcodec_find_decoder(m_codec_parametrs->codec_id);
err = avcodec_parameters_to_context(m_pCodecCtx, m_codec_parametrs);
if (err < 0) {
	Debug::Error("Video::Load() : ffmpeg: failed convert parameters to context!");
	return false;
}
AVCodec* codec = avcodec_find_decoder(m_pCodecCtx->codec_id);

m_dst_width = m_codec_parametrs->width;//codec_parametrs->width;
m_dst_height = m_codec_parametrs->height;//codec_parametrs->height;
m_dst_pix_fmt = AV_PIX_FMT_YUV420P; //AV_PIX_FMT_BGR24; //(AVPixelFormat)codec_parametrs->format; //;

std::cout << m_dst_width << " " << m_dst_height << std::endl;

int size = av_image_get_buffer_size(
	m_dst_pix_fmt,
	m_dst_width,
	m_dst_height,
	1
);

while (true) {
	AVPacket* packet = av_packet_alloc();

	if (av_read_frame(this->m_format_context, packet) == AVERROR_EOF)
		break;

	//av_init_packet(packet);
	//av_new_packet(packet, size);

	std::cout << av_error(avcodec_send_packet(this->m_pCodecCtx, packet)) << std::endl;
	*/
/*
av_init_packet(packet);
av_parser_parse2(parser, cctx,
	&(packet->data), &(packet->size),
	tempPacket->data, tempPacket->size,
	tempPacket->pts, tempPacket->dts, tempPacket->pos
);

packet->pts = parser->pts;
packet->dts = parser->dts;
packet->pos = parser->pos;

//Set keyframe flag
if (parser->key_frame == 1 ||
	(parser->key_frame == -1 &&
		parser->pict_type == AV_PICTURE_TYPE_I))
	packet->flags |= AV_PKT_FLAG_KEY;
if (parser->key_frame == -1 && parser->pict_type == AV_PICTURE_TYPE_NONE && (packet->flags & AV_PKT_FLAG_KEY))
	packet->flags |= AV_PKT_FLAG_KEY;
packet->duration = 96000; //Same result as in av_read_frame()
	*/

/*
	AVFrame* decframe = createFrame(packet);
	if (!decframe) {
		av_packet_unref(packet);
		continue;
	}

	std::cout << decframe->width << std::endl;
	*/

//av_packet_unref(packet);

//av_free(decframe);
//av_frame_unref(decframe);
//}
/*
SwsContext* swsctx = sws_getCachedContext(
	nullptr, m_dst_width, m_dst_height, m_dst_pix_fmt,
	m_dst_width, m_dst_height, m_dst_pix_fmt, SWS_BICUBIC, nullptr, nullptr, nullptr);
if (!swsctx) {
	Debug::Error("Video::Load() : failed to sws_getCachedContext!");
	return false;
}



std::vector<uint8_t> framebuf(size);
err = av_image_fill_arrays(frame->data,			// destination
			frame->linesize,					// destination
			framebuf.data(),					// source
			m_dst_pix_fmt,						// source
			m_dst_width,						// source & alingment
			m_dst_height, 1
);
if (err < 0) {
	Debug::Error("Video::Load() : av_image_fill_arrays returned " + std::to_string(err));
	return false;
}

AVFrame* decframe = av_frame_alloc();
{ // video while
	avcodec_receive_frame(this->m_pCodecCtx, decframe);
	sws_scale(swsctx, decframe->data, decframe->linesize, 0, decframe->height, frame->data, frame->linesize);
	std::cout << framebuf.data() << std::endl;
}


// Free the YUV frame
//av_free(frame);

// Close the codec
//avcodec_close(codec_context);

// Close the video file
//avformat_close_input(&format_context);
*/

/*
int buffer_size = av_image_get_buffer_size(this->m_dst_pix_fmt, m_dst_width, m_dst_height, 1);
uint8_t* buffer = (uint8_t*)av_malloc(buffer_size * sizeof(uint8_t));
memset(buffer, 1, buffer_size);

if (packet->stream_index == this->m_video_stream) {
	// Send the data packet to the decoder

	int sendPacketResult = avcodec_send_packet(m_pCodecCtx, packet);
	if (sendPacketResult == AVERROR(EAGAIN)) {
		// Decoder can't take packets right now. Make sure you are draining it.
		av_free(buffer);
		return nullptr;
	}
	else if (sendPacketResult < 0) {
		// Failed to send the packet to the decoder
		av_free(buffer);
		return nullptr;
	}

	//!=================================

	// Get decoded frame from decoder
	AVFrame* frame = av_frame_alloc();
	int decodeFrame = avcodec_receive_frame(m_pCodecCtx, frame);

	if (decodeFrame == AVERROR(EAGAIN)) {
		// The decoder doesn't have enough data to produce a frame
		// Not an error unless we reached the end of the stream
		// Just pass more packets until it has enough to produce a frame
		//av_frame_unref(frame);
		//av_freep(frame);
	}
	else if (decodeFrame < 0) {
		// Failed to get a frame from the decoder
		//av_frame_unref(frame);
		//av_freep(frame);
	}

	av_free(buffer);
	return frame;
}
else {
	return nullptr;
}
*/

/*
av_image_fill_arrays(
	m_framebuf.data(),
	m_frame->linesize,
	buffer,
	m_dst_pix_fmt, m_dst_width, m_dst_height, 1
);

AVFrame* frame = av_frame_alloc();
frame->format = m_dst_pix_fmt;
frame->width = m_dst_width;
frame->height = m_dst_height;
av_frame_get_buffer(frame, 0);
av_image_copy(frame->data, frame->linesize,
const_cast<const uint8_t**>(m_framebuf.data()), const_cast<const int*>(m_frame->linesize),
	m_dst_pix_fmt, m_dst_width, m_dst_height);

*/
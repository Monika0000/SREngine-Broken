#include "pch.h"
#include "Video.h"
extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"
}

bool SpaRcle::Graphics::Video::Load()
{
	AVFormatContext* format_context = NULL;
	int err = avformat_open_input(&format_context, m_file_name.c_str(), NULL, NULL);
	if (err < 0) {
		Debug::Error("Video::Load() : ffmpeg: Unable to open input file");
		return false;
	}

	// Retrieve stream information
	err = avformat_find_stream_info(format_context, NULL);
	if (err < 0) {
		Debug::Error("Video::Load() : ffmpeg: Unable to find stream info");
		return false;
	}

	// Find the first video stream
	int video_stream;
	for (video_stream = 0; video_stream < format_context->nb_streams; ++video_stream) {
		if (format_context->streams[video_stream]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
			break;
		}
	}
	if (video_stream == format_context->nb_streams) {
		Debug::Error("Video::Load() : ffmpeg: Unable to find video stream");
		return false;
	}

	//AVCodecParameters* codec_context = format_context->streams[video_stream]->codecpar;
	////AVCodec* codec = avcodec_find_decoder(codec_context->codec_id);
	//err = avcodec_open2(codec_context, codec, NULL);
	//if (err < 0) {
	//	Debug::Error("Video::Load() : ffmpeg: Unable to open codec\n");
	//	return false;
	//}

	AVFrame* frame = av_frame_alloc();
	AVPacket packet;
	while (av_read_frame(format_context, &packet) >= 0) {
		std::cout << packet.data << std::endl;
	}

	// Free the YUV frame
	//av_free(frame);

	// Close the codec
	//avcodec_close(codec_context);

	// Close the video file
	//avformat_close_input(&format_context);


	return true;
}

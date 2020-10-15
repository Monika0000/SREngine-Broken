#pragma once
#include <iostream>
#include <string>
#include "Material.h"

//#include <SDL/SDL.h>

//#include <libswscale/swscale.h>

#include <Debug.h>

#pragma comment(lib, "avcodec.lib")
#pragma comment(lib, "avdevice.lib")
#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "avutil.lib")
#pragma comment(lib, "swscale.lib")
/*
#pragma comment(lib, "avfilter.lib")
#pragma comment(lib, "swscale.lib")
#pragma comment(lib, "swresample.lib")
*/

//enum AVPixelFormat;

struct SwsContext;
struct AVPacket;
struct AVFormatContext;
struct AVCodecContext;
struct AVFrame;

namespace SpaRcle {
	using namespace Helper;

	namespace Graphics {
		class ResourceManager;
		class Video : public Material {
			friend class ResourceManager;
		public:
			enum class PlayMode {
				Unknown, AutoPlay, PlayOnUse, NonPlay
			};
		private:
			//int					m_dst_width				= 0;
			//int					m_dst_height				= 0;
			PlayMode				m_playMode				= PlayMode::Unknown;
			std::string				m_file_name				= "";
			AVCodecContext*			m_pCodecCtx				= nullptr;
			SwsContext*				m_sws_ctx				= nullptr;
			//AVPixelFormat			m_dst_pix_fmt				= AVPixelFormat();
			AVFrame*				m_pFrame				= nullptr;
			AVFrame*				m_pFrameRGB				= nullptr;
			int						m_video_stream			= -1;
			AVFormatContext*		m_pFormatCtx			= nullptr;
			uint8_t*				m_buffer				= nullptr;
		private:
			bool Load();
			AVFrame* createFrame(AVPacket*	 packet);
		private:
			~Video() {

			}
			Video(std::string file_name, PlayMode play_mode);
		public:
			void Use() override {

			}
		};
	}
}


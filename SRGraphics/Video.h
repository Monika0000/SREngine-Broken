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
				Unknown, RepeetOnUse, OnePlayOnUse, NonPlay
			};
			enum class RenderMode {
				CalculateInRealTime,    // low speed, non use memory
				PreCalculate			// hight speed, use many memory
			};
		private:
			std::vector<GLuint>							m_calculate_frames		= {};
			std::vector<std::vector<unsigned char>>		m_source_frames			= {};
		private:
			size_t										m_count_frames			= 0;
			size_t										m_current_frame			= 0;
			unsigned short								m_frames_per_second		= 0;
			int											m_width					= 0;
			int											m_channels				= 0;
			int											m_height				= 0;
			PlayMode									m_playMode				= PlayMode::Unknown;
			RenderMode									m_renderMode			= RenderMode::PreCalculate;
			std::string									m_file_name				= "";

			GLuint										m_frame_id				= 0;
			bool										m_is_calculate			= false;
			bool										m_is_finished			= false;
		private:
			bool Calculate();
			bool Load();
		private:
			~Video() {

			}
			Video(std::string file_name, PlayMode play_mode, RenderMode renderMode);
		public:
			void Use() override;
		};
	}
}


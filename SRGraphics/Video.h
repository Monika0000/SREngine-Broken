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
		class Mesh;
		class Video : public Material {
			friend class ResourceManager;
			friend class Mesh;
		public:
			enum class PlayMode {
				Unknown, RepeetOnUse, OnePlayOnUse, NonPlay
			};
			enum class RenderMode {
				CalculateInRealTime,    // low speed, non use memory
				PreCalculate			// hight speed, use many memory
			};
		private:
			double m_maxFPS				= 0;//60.0;
			double m_maxPeriod			= 0;//1.0 / maxFPS;

			// approx ~ 16.666 ms

			double m_lastTime			= 0.0;
		private:
			//std::vector<GLuint>							m_calculate_frames		= {};
			std::vector<std::vector<unsigned char>>		m_source_frames			= {};
		private:
			GLuint										m_defalt_texture		= 0;
			size_t										m_count_frames			= 0;
			size_t										m_current_frame			= 0;
			size_t										m_old_frame				= -1;

			unsigned short								m_frames_per_second		= 0;
			int											m_width					= 0;
			int											m_channels				= 0;
			int											m_height				= 0;
			PlayMode									m_playMode				= PlayMode::Unknown;
			RenderMode									m_renderMode			= RenderMode::PreCalculate;
			std::string									m_file_name				= "";

			unsigned char*								m_data					= nullptr;
			GLuint										m_frame_id				= 0;
			bool										m_is_calculate			= false;
			bool										m_is_finished			= false;

			bool										m_thread_use			= false;
			bool										m_thread_next			= false;
		private:
			bool Calculate();
			bool Load();
		protected:
			bool Destroy() override;
		private:
			~Video() {
			ret: if (m_thread_next) goto ret;
				this->m_file_name.clear();
			}
			Video(std::string file_name, PlayMode play_mode, RenderMode renderMode = RenderMode::CalculateInRealTime);
		public:
			const std::string GetFileName() const { return m_file_name; }
			bool NextFrame(); 
			inline const float GetVideoFormat() const { return (float)m_width / (float)m_height; }
			inline const bool IsFinished() const { return m_is_finished; }
			bool Use() override;
		};
	}
}


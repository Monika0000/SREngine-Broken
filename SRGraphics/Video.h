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
/*
#pragma comment(lib, "avfilter.lib")
#pragma comment(lib, "swscale.lib")
#pragma comment(lib, "swresample.lib")
*/

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
			inline volatile static bool m_is_register = false;
			PlayMode m_playMode = PlayMode::Unknown;
			std::string m_file_name = "";
		private:
			bool Load();
		private:
			~Video() {
			}
			Video(std::string file_name, PlayMode play_mode) : Material(false) {
				this->m_playMode	= play_mode;
				this->m_file_name	= file_name;

				if (!m_is_register) {
					Debug::Info("Video::Constructor() : register audio player...");

					//av_register_all();

					m_is_register = true;
				}

				if (!Load())
					Debug::Error("Video::Constructor() : failed load video \"" + m_file_name + "\", this data structure delete needed!");
			}
		public:
			void Use() override {

			}
		};
	}
}


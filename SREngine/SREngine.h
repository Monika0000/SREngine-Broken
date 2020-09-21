#pragma once
#define GLEW_STATIC
#define NOMINMAX

#include <Debug.h>
#include <Render.h>
#include <Window.h>
#include <SRGraphics.h>
#include <string>

using namespace SpaRcle::Graphics;

namespace SpaRcle {
	namespace Engine {
		class SREngine {
		private:
			SREngine() { };
			~SREngine() { };
		private:
			std::string				m_resource_folder		= "";
			SRGraphics*				m_graph					= nullptr;
		private:
			volatile bool			m_isCreated				= false;
			volatile bool			m_isInit				= false;
			volatile bool			m_isRunning				= false;
		public:
			static SREngine* Get() {
				static SREngine* engine = nullptr;
				if (!engine)
					engine = new SREngine();
				return engine;
			}
		public:
			bool Create(Window* win, std::string resource_folder);
			bool Init();
			bool Run();
			bool Close();
		};
	}
}


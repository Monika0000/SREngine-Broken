#pragma once
#define GLEW_STATIC
#define NOMINMAX

#include <Debug.h>
#include <Render.h>
#include <Window.h>
#include <SRGraphics.h>
#include <string>

#include "Compiler.h"

#include <GameObject.h>
#include <Transform.h>

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
			Window*					m_window				= nullptr;
			Compiler*				m_compiler				= nullptr;
			GameObject*				m_camera				= nullptr;
		private:
			GameObject*				m_arrows_tool			= nullptr;
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
		private:
			bool InitEngineGUI();
		private:
			bool ProcessKeyboard();
			void ProcessMouse();
		public:
			bool Create(Window* win);
			bool Init();
			bool Run();
			bool Close();
		};
	}
}


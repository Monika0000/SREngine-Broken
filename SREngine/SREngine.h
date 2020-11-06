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
		enum class Tool {
			None, Moving, Rotating, Scaling 
		};

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
			volatile Tool			m_current_tool			= Tool::None;
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
			const bool CurrentToolIsMoving()		const { return m_current_tool == Tool::Moving;		};
			const bool CurrentToolIsRotating()		const { return m_current_tool == Tool::Rotating;	};
			const bool CurrentToolIsScaling()		const { return m_current_tool == Tool::Scaling;		};
			void SetTool(Tool tool) { this->m_current_tool = tool; }
		public:
			bool Create(Window* win);
			bool Init();
			bool Run();
			bool Close();
		};
	}
}


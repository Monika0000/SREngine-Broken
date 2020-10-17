#pragma once
#include "Window.h"

#include <thread>

namespace SpaRcle {
	using namespace Helper;

	namespace Graphics {
		class SRGraphics {
		private:
			std::string		m_resource_folder		= "";
			Window*			m_window				= nullptr;
			std::thread		m_update_task			= std::thread();
		private:
		 	volatile bool	m_isCreated				= false;
			volatile bool	m_isInit				= false;
			volatile bool	m_isRunning				= false;
		private:
			void FixedUpdate();
		public:
			Window* GetMainWindow() {
				if (m_window) return m_window;
				else {
					Debug::Error("SRGraphics::GetMainWindow() : m_window is nullptr!");
					return nullptr;
				}
			}
		public:
			static SRGraphics* Get() {
				static SRGraphics* graph = nullptr;
				if (!graph) {
					graph = new SRGraphics();
				}
				return graph;
			}
		public:
			bool Create(Window*win, std::string resource_folder);
			bool Init();
			bool Run();
			bool Close();
		};
	}
}


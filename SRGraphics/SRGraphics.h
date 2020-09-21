#pragma once
#include "Window.h"

namespace SpaRcle {
	using namespace Helper;

	namespace Graphics {
		class SRGraphics {
		private:
			Window*			m_window				= nullptr;
		private:
		 	volatile bool	m_isCreated				= false;
			volatile bool	m_isInit				= false;
			volatile bool	m_isRunning				= false;
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
			bool Create(Window*win);
			bool Init();
			bool Run();
			bool Close();
		};
	}
}


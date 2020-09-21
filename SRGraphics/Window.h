#pragma once
#define GLEW_STATIC
#define NOMINMAX
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <thread>
#include <Windows.h>

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "SOIL.lib")

#include <Debug.h>

#include "Camera.h"
#include "Render.h"

namespace SpaRcle {
	namespace Graphics {
		class Window {
		public:
			enum class Format {
				Unknown, _1600_900
			};
		private:
			Render*			m_render			= nullptr;
			Camera*			m_camera			= nullptr;
		private:
			std::thread		m_win_task			= std::thread();
			Format			m_format			= Format::Unknown;
			bool			m_movable			= false;
			bool			m_mouseLock			= false;
			bool			m_esc_key_active	= false;
		private:
			volatile bool	m_isCreated			= false;
			volatile bool	m_isInit			= false;
			volatile bool	m_isWindowRun		= false;
			volatile bool	m_isRunning			= false;
		public:
			Window(Render* render, Camera* camera, Format format, bool movable, bool mouseLock, bool esc_key_active) {
				this->m_format			= format;
				this->m_movable			= movable;
				this->m_mouseLock		= mouseLock;
				this->m_esc_key_active	= esc_key_active;
			};
			~Window() { };
		private:
			bool InitGlfw();
			bool InitGlew();
			bool InitGlut();
			bool InitGL_Parametrs();
			bool RunOpenGLWindow();
		public:
			void WaitWindowRunning() noexcept {
			ret:
				if (!m_isWindowRun)
					goto ret;
				else
					return;
			}
		public:
			bool Create();
			bool Init();
			bool Run();
			void Close();
		};
	}
}


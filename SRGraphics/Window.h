#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define GLEW_STATIC
#define NOMINMAX
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <thread>
#include <Windows.h>

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "SOIL.lib")

#include <Debug.h>

#include "Camera.h"
#include "Render.h"

namespace SpaRcle {
	using namespace Helper;

	namespace Graphics {
		class Window {
		public:
			enum class FormatType {
				Unknown,
				_640_360,
				_1280_720,
				_1600_900
			};
			class Format {
			private:
				Format() {}; ~Format() {};
			public:
				static const unsigned int GetWidth(FormatType format) noexcept {
					switch (format) {
					case FormatType::Unknown:		return 0;
					case FormatType::_640_360:		return 640;
					case FormatType::_1280_720:		return 1280;
					case FormatType::_1600_900:		return 1600;
					default: Debug::Error("Format::GetWidth() : unknown type " + std::to_string((int)format) + "!"); return 0;
					}
				}
				static const unsigned int GetHeight(FormatType format) {
					switch (format) {
					case FormatType::Unknown:		return 0;
					case FormatType::_640_360:		return 360;
					case FormatType::_1280_720:		return 720;
					case FormatType::_1600_900:		return 900;
					default: Debug::Error("Format::GetHeight() : unknown type " + std::to_string((int)format) + "!"); return 0;
					}
				}
			};
		private:
			Render* m_render = nullptr;
			Camera* m_camera = nullptr;
		private:
			glm::vec2			m_screen_size = glm::vec2();
			GLFWwindow*			m_glfw_window = nullptr;
			std::thread			m_win_task = std::thread();
			glm::mat4			m_projection = glm::mat4(1);

			// ----------------------- CONFIGURATIONS ---------------------------
			FormatType			m_format = FormatType::Unknown;
			char*				m_argv = nullptr;
			int					m_argcp = 0;
			const char*			m_win_name = nullptr;
			const unsigned char m_smooth_samples = 0;

			bool				m_movable = false;
			bool				m_mouseLock = false;
			bool				m_vsync = false;
		private:
			volatile bool		m_isCreated = false;
			volatile bool		m_isInit = false;
			volatile bool		m_isWindowRun = false;
			volatile bool		m_isRunning = false;
		public:
			Window(
				const char* win_name,
				int				argcp,
				char* argv,
				Render* render,
				Camera* camera,
				FormatType		format,
				bool			movable,
				bool			mouseLock,
				bool			vsync,
				unsigned char	smooth_samples = 4
			) :
				m_win_name(win_name),
				m_smooth_samples(smooth_samples)
			{
				this->m_argcp = argcp;
				this->m_argv = argv;

				this->m_render = render;
				this->m_camera = camera;

				this->m_format = format;
				this->m_movable = movable;
				this->m_mouseLock = mouseLock;
				this->m_vsync = vsync;
			};
			~Window() { };
		private:
			static const glm::vec2 GetScreenResolution() noexcept {
				glm::vec2 size = glm::vec2();

				RECT desktop;
				// Get a handle to the desktop window
				const HWND hDesktop = GetDesktopWindow();
				// Get the size of screen to the variable desktop
				GetWindowRect(hDesktop, &desktop);
				// The top left corner will have coordinates (0,0)
				// and the bottom right corner will have coordinates
				// (horizontal, vertical)
				size.x = desktop.right;
				size.y = desktop.bottom;

				return size;
			}
		private:
			void Draw();
			void PoolEvents();
		private:
			bool InitGlfw();
			bool InitGlew();
			bool InitGlut();
			bool InitGL_Parametrs();
			bool RunOpenGLWindow();
		public:
			glm::mat4& GetProjection() { return this->m_projection; }
			FormatType GetFormat() { return this->m_format; }
			Camera* GetCamera();
			Render* GetRender();
		public:
			const void WaitWindowRunning() const noexcept {
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


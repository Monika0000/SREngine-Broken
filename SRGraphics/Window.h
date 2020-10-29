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
#include "GameObject.h"
#include "PostProcessing.h"

struct ImVec2;
namespace SpaRcle {
	using namespace Helper;

	namespace Graphics {
		class PostProcessing;

		class Window {
			friend class PostProcessing;
		public:
			enum class FormatType {
				Unknown,
				_640_360,
				_1280_720,
				_1366_768,
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
					case FormatType::_1366_768:		return 1366;
					case FormatType::_1600_900:		return 1600;
					default: Debug::Error("Format::GetWidth() : unknown type " + std::to_string((int)format) + "!"); return 0;
					}
				}
				static const unsigned int GetHeight(FormatType format) {
					switch (format) {
					case FormatType::Unknown:		return 0;
					case FormatType::_640_360:		return 360;
					case FormatType::_1280_720:		return 720;
					case FormatType::_1366_768:		return 768;
					case FormatType::_1600_900:		return 900;
					default: Debug::Error("Format::GetHeight() : unknown type " + std::to_string((int)format) + "!"); return 0;
					}
				}
			};
		private:
			PostProcessing*		m_post_processing			= nullptr;
			Shader*				m_post_processing_shader	= nullptr;
			GLuint				m_FBO						= 0;
			GLuint				m_RBO						= 0;
			GLuint				m_ScreenTexture				= 0;

			int					m_FPS						= 0;

			Render*				m_render					= nullptr;
			Camera*				m_camera					= nullptr;
			GameObject*			m_camera_gm					= nullptr;
		private:
			/* Значения размера экрана */
			glm::vec2			m_screen_size				= glm::vec2();
			/* Значения размера окна программы (Не окна внутри движка!!!) */
			glm::vec2			m_window_format_size		= glm::vec2();
			/* Значение размера окна сцены внутри окна приложения, используется при выключенном рендере в окно в пост процессинге */
			//glm::vec2			m_scene_window_size			= glm::vec2();

			GLFWwindow*			m_glfw_window				= nullptr;
			std::thread			m_win_task					= std::thread();
			glm::mat4			m_projection				= glm::mat4(1);
			HWND				m_hWnd						= NULL;
			HWND				m_win32_hWnd				= NULL;
			HDC					m_hDC						= NULL;
			// ----------------------- CONFIGURATIONS ---------------------------
			FormatType			m_format					= FormatType::Unknown;
			char*				m_argv						= nullptr;
			int					m_argcp						= 0;
			const char*			m_win_name					= nullptr;
			const unsigned char m_smooth_samples			= 0;

			bool				m_movable					= false;
			bool				m_mouseLock					= false;
			bool				m_vsync						= false;
		private:
			volatile bool		m_isCreated					= false;
			volatile bool		m_isInit					= false;
			volatile bool		m_isWindowRun				= false;
			volatile bool		m_isRunning					= false;
			volatile bool		m_isFailedRunning			= false;
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
			);
			~Window() { };
		public:
			/*	
				Получить размер окна программы для создания док окна на весь экран (Для интерфейса движка)
				ЭТО НЕ РАЗМЕР ОКНА СЦЕНЫ 
			*/
			ImVec2 GetDockSpace();
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
			bool InitWin32Window();
			bool InitGlfw();
			bool InitGlew();
			bool InitGlut();
			bool InitGL_Parametrs();
			bool RunOpenGLWindow();
		public:
			bool IsFocusedWindow();
		public:
			/* Call only from resize method! */
			void SetWindowFormatSize(int w, int h) { this->m_window_format_size = { w, h }; }
			
			/* Call only from post processing */
			GLuint* GetFBO() { return &this->m_FBO; }
			/* Call only from post processing */
			GLuint* GetScreenTexture() { return &this->m_ScreenTexture; }

			//void SetSceneWindowSize(int w, int h) { this->m_scene_window_size = { w,h }; }
			//glm::vec2 GetSceneWindowSize() { return this->m_scene_window_size; }
		public:
			void ResizeWindow(int w, int h);
			//void ResizeSceneWindow(int w, int h);
		public:
			int GetFPS() const { return this->m_FPS; };
			PostProcessing* GetPostProcessing() { return this->m_post_processing; }
			GLFWwindow* GetGLFWWindow() {
				if (this->m_glfw_window)
					return m_glfw_window;
				else {
					Debug::Error("Window::GetGLFWWindow() : m_glfw_window is nullptr!");
					return nullptr;
				}
			}
			HWND GetWin32HWND() {
				if (this->m_win32_hWnd)
					return m_win32_hWnd;
				else {
					Debug::Error("Window::GetWin32HWND() : m_win32_hWnd is NULL!");
					return NULL;
				}
			}
			HWND GetHWND() {
				if (m_hWnd)
					return m_hWnd;
				else {
					Debug::Error("Window::GetHWND() : m_hWnd is NULL!");
					return NULL;
				}
			}
			glm::mat4& GetProjection() { return this->m_projection; }
			FormatType GetFormat() { return this->m_format; }
			GameObject* GetCameraGameObject();
			Camera* GetCamera();
			Render* GetRender();
		public:
			bool ReCalcFBO(int width, int height);
			const void WaitWindowRunning() const noexcept {
			ret:
				if (!m_isWindowRun)
					goto ret;
				else
					return;
			}
			inline void CentralizeMouse() {
				SetCursorPos(m_screen_size.x / 2, m_screen_size.y / 2);
			}
			void MouseLock(const bool val) {
				if (val)
					glfwSetInputMode(m_glfw_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
				else
					glfwSetInputMode(m_glfw_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				m_mouseLock = val;
			};
			const bool MouseLock() const { return m_mouseLock; };
		public:
			bool Create();
			bool Init();
			bool Run();
			void Close();
		};
	}
}


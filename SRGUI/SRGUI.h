#pragma once
#include <Windows.h>
#include <string>

struct GLFWwindow;

namespace SpaRcle {
	namespace GUI {
		class SRGUI {
		private:
			inline static GLFWwindow* m_glfw_window = nullptr;
		public:
			static bool InitOptions();

			static bool AttachGLFW(GLFWwindow* glfw_window);

			static bool LoadFile(std::string path);

			static bool Draw();

			static bool Destroy();
		};
	}
}


#pragma once
#include <Windows.h>
#include <string>

struct GLFWwindow;

namespace SpaRcle {
	namespace GUI {
		class SRGUI {
		private:
			SRGUI() {};
			~SRGUI() {};
		public:
			/* call only from glfw context */
			bool Init(GLFWwindow* window);
		public:
			static SRGUI* Get() {
				static SRGUI* gui = nullptr;
				if (!gui) {
					gui = new SRGUI();
				}
				return gui;
			}
		};
	}
}


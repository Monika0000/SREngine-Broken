#pragma once
#define GLEW_STATIC
#define NOMINMAX

#include <iostream>
#include <thread>

#include <glm/glm.hpp>

#include "Component.h"

namespace SpaRcle {
	namespace Graphics {
		class Component;
		class Shader;
		class Window;

		class Camera : public Component {
		public:
			Camera() {

			}
			~Camera() {

			}
		private:
			bool			m_isCreated			= false;
			bool			m_isInit			= false;
			bool			m_isRunning			= false;
		private:
			Window*			m_window			= nullptr;
			glm::mat4*		m_projection		= nullptr;
			//std::thread		m_task				= std::thread();
			//glm::vec3		m_position			= glm::vec3();;
		public:
			void UpdateShader(Shader* shader) noexcept;
		public:
			bool Create(Window* window);
			bool Init();
			bool Run();
			bool Close();
		//public:
		//	void Move(glm::vec3 dir) {
		//
		//	}
		};
	}
}


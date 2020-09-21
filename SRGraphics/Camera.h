#pragma once
#define GLEW_STATIC
#define NOMINMAX

#include <iostream>
#include <thread>

#include <glm/glm.hpp>

namespace SpaRcle {
	namespace Graphics {
		class Camera {
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
			std::thread		m_task				= std::thread();
			glm::vec3		m_position			= glm::vec3();;
		public:
			bool Create();
			bool Init();
			bool Run();
			bool Close();
		public:
			void Move(glm::vec3 dir) {

			}
		};
	}
}


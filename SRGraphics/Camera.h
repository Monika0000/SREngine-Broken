#pragma once
#define GLEW_STATIC
#define NOMINMAX

//////////////////////////////////
#define _CRT_SECURE_NO_WARNINGS

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <thread>
#include <Windows.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "SOIL.lib")
//////////////////////////////////

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
			void OnMoved(glm::vec3 new_pos) override;
			void OnRotated(glm::vec3 new_rot) override;
		private:
			bool			m_isCreated			= false;
			bool			m_isInit			= false;
			bool			m_isRunning			= false;
		private:
			Window*			m_window			= nullptr;
			glm::mat4*		m_projection		= nullptr;
			glm::mat4		m_viewMat			= glm::mat4(0);
			glm::vec3		m_pos				= {0,0,0};
			volatile float	m_yaw				= 0;
			volatile float	m_ptc				= 0;
			float			m_dxx				= 0;
			float			m_dxz				= 0;
			float			m_dyy				= 0;
			//std::thread		m_task				= std::thread();
			//glm::vec3		m_position			= glm::vec3();;
		private:
			void UpdateView();
		public:
			void UpdateShader(Shader* shader) noexcept;
		public:
			const char* TypeName() override { return "Camera"; }
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


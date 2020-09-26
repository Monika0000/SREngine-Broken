#pragma once
#include <string>
#include <GL\glew.h>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm\ext\matrix_transform.hpp>

namespace SpaRcle {
	namespace Graphics {
		class Render;
		class Camera;

		class Shader {
		private:
			Render*			m_render		= nullptr;
			GLuint			m_ProgramID		= 0;
			std::string		m_name			= "";
			bool			m_isDestroy		= false;
			bool			m_isLinked		= false;
			bool			m_isCompiled	= false;
		public:
			Shader(std::string name, Render* render);
			~Shader() {
				if (!m_isDestroy) this->Destroy();
			}
		public:
			Camera* GetCamera();
		public:
			bool Linking();
			bool Compile();
			bool Use();
			bool Destroy();
		};
	}
}


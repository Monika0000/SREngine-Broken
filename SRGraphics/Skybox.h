#pragma once
#include "Texture.h"
#include "Shader.h"
#include <Input.h>

namespace SpaRcle {
	namespace Graphics {
		class ResourceManager;
		
		class Skybox {
			friend class ResourceManager;
			const float skyboxVertices[36 * 3] = {
				// positions          
				-10.0f,  10.0f, -10.0f,
				-10.0f, -10.0f, -10.0f,
				 10.0f, -10.0f, -10.0f,
				 10.0f, -10.0f, -10.0f,
				 10.0f,  10.0f, -10.0f,
				-10.0f,  10.0f, -10.0f,

				-10.0f, -10.0f,  10.0f,
				-10.0f, -10.0f, -10.0f,
				-10.0f,  10.0f, -10.0f,
				-10.0f,  10.0f, -10.0f,
				-10.0f,  10.0f,  10.0f,
				-10.0f, -10.0f,  10.0f,

				 10.0f, -10.0f, -10.0f,
				 10.0f, -10.0f,  10.0f,
				 10.0f,  10.0f,  10.0f,
				 10.0f,  10.0f,  10.0f,
				 10.0f,  10.0f, -10.0f,
				 10.0f, -10.0f, -10.0f,

				-10.0f, -10.0f,  10.0f,
				-10.0f,  10.0f,  10.0f,
				 10.0f,  10.0f,  10.0f,
				 10.0f,  10.0f,  10.0f,
				 10.0f, -10.0f,  10.0f,
				-10.0f, -10.0f,  10.0f,

				-10.0f,  10.0f, -10.0f,
				 10.0f,  10.0f, -10.0f,
				 10.0f,  10.0f,  10.0f,
				 10.0f,  10.0f,  10.0f,
				-10.0f,  10.0f,  10.0f,
				-10.0f,  10.0f, -10.0f,

				-10.0f, -10.0f, -10.0f,
				-10.0f, -10.0f,  10.0f,
				 10.0f, -10.0f, -10.0f,
				 10.0f, -10.0f, -10.0f,
				-10.0f, -10.0f,  10.0f,
				 10.0f, -10.0f,  10.0f
			};
		private:
			bool			m_is_calculate		= false;
			bool			m_is_destroy		= false;
			GLuint			m_VAO				= 0;
			GLuint			m_Cubemap			= 0;
			//Camera*			m_camera			= nullptr;
		private:
			std::vector<Image*> m_sides		= {};
		private:
			bool Calculate();
		private:
			Skybox(std::vector<Image*> sides) {
				this->m_sides = sides;
			}
			~Skybox() {

			}
		public:
			//void SetCamera(Camera* cam);
			void Draw();
			/* call only from OpenGL context */
			bool Destroy();
		};
	}
}


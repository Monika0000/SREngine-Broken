#pragma once
#include <Debug.h>

typedef unsigned int GLuint;

namespace SpaRcle {
	namespace Graphics {
		class Render;
		class Shader;
		class Window;

		/*
			All methods calling from OpenGL context
		*/

		class PostProcessing {
			// vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
			inline static const float QuadVertices[] = { 
				 // positions   // texCoords
				 -1.0f,  1.0f,  0.0f, 1.0f,
				 -1.0f, -1.0f,  0.0f, 0.0f,
				  1.0f, -1.0f,  1.0f, 0.0f,

				 -1.0f,  1.0f,  0.0f, 1.0f,
				  1.0f, -1.0f,  1.0f, 0.0f,
				  1.0f,  1.0f,  1.0f, 1.0f
			};
		private:
			Window*		m_window			= nullptr;
			Shader*		m_shader			= nullptr;
			GLuint*		m_FBO				= nullptr;
			GLuint*		m_ScreenTexture		= nullptr;
			GLuint		m_VAO				= 0;
			bool		m_isInit			= false;
			bool		m_isCaclucale		= false;
		public:
			PostProcessing(Window* window);
			~PostProcessing();
		public:
			bool Init(Shader* shader, GLuint* FBO, GLuint* screen_texture);
			bool Destoy();
		public:
			bool Begin();
			bool Calculate();
			bool End();
		};
	}
}

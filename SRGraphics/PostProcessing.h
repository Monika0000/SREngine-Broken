#pragma once
#include <Debug.h>
#include "Component.h"

typedef unsigned int GLuint;

namespace SpaRcle {
	namespace Graphics {
		class Render;
		class Shader;
		class Window;

		/*
			All methods calling from OpenGL context
		*/

		class PostProcessing : public Component {
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
			float			m_gamma					= 0.8f;
			glm::vec3		m_color_correction		= { 1, 1, 1 };
		private:
			GLuint			m_screen_pp_texture		= 0;
			GLuint			m_pp_FBO				= 0;
			GLuint			m_pp_RBO				= 0;

			Window*			m_window				= nullptr;
			Shader*			m_shader				= nullptr;
			GLuint*			m_FBO					= nullptr;
			GLuint*			m_ScreenTexture			= nullptr;
			GLuint			m_VAO					= 0;
			GLuint			m_VBO					= 0;
			bool			m_isInit				= false;
			bool			m_isCaclucale			= false;
			bool			m_isDestroy 			= false;
			volatile bool	m_render_into_window    = true;
		public:
			const char* TypeName() override { return "PostProcessing"; }
		public:
			glm::vec3 GetColorCorrection() const { return m_color_correction; }
			void SetColorCorrection(glm::vec3 col) { this->m_color_correction = col; }

			float GetGamma() const { return m_gamma; }
			void SetGamma(float gamma) { this->m_gamma = gamma; }
		public:
			bool IsEnabledRenderIntoWindow() const { return m_render_into_window; }
			void DisableRenderIntoWindow() { this->m_render_into_window = false; }
			void EnableRenderIntoWindow() { this->m_render_into_window = true; }
		public:
			void Resize(int w, int h);
			GLuint GetScreenTexture() const { return m_screen_pp_texture; }
			bool Use();
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

#pragma once
#define GLEW_STATIC
#define NOMINMAX
#include <GL\glew.h>

namespace SpaRcle {
	namespace Graphics {
		class Texture {
			enum class Type {
				Diffuse, Normal, Specular, Roughness, Glossiness
			};
			enum class Filter {
				NEAREST = GL_NEAREST, LINEAR = GL_LINEAR, NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
				LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST, NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR, LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR,
			};
		private:
			Type		m_type				= Type::Diffuse;
			Filter		m_filter			= Filter::LINEAR;
		private:
			bool		m_is_calculated		= false;
			GLuint		m_id				= 0;
		private:
			bool Calculate();
		};
	}
}


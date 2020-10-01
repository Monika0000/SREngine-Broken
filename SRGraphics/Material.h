#pragma once
#include "Texture.h"

namespace SpaRcle {
	namespace Graphics {
		class Material {
		public:
			Material(bool transparent) : m_transparent(transparent) {

			}
		private:
			const bool	m_transparent = false;
		private:
			Texture*	m_diffuse			= nullptr;
			Texture*	m_normal			= nullptr;
			Texture*	m_specular			= nullptr;
			Texture*	m_glossiness		= nullptr;
		public:
			const bool IsTransparent() const noexcept { return m_transparent; }
			void Use();
		};
	}
}


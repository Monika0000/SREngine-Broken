#pragma once
#include "Texture.h"

namespace SpaRcle {
	namespace Graphics {
		class ResourceManager;
		class Shader;
		class Material {
			friend class ResourceManager;
		protected:
			~Material() {

			}
			Material(bool transparent, Shader* shader);
			Material(bool transparent, Shader* shader, std::vector<Texture*> textures);
		protected:
			const bool	m_transparent		= false;
			bool		m_is_destroy		= false;
		protected:
			virtual bool Destroy();
		protected:
			Shader*		m_fragment_shader	= nullptr;
		private:
			Texture*	m_diffuse			= nullptr;
			Texture*	m_normal			= nullptr;
			Texture*	m_specular			= nullptr;
			Texture*	m_glossiness		= nullptr;
		public:
			const bool IsTransparent() const noexcept { return m_transparent; }
			virtual bool Use();
		};
	}
}


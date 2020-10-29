#pragma once
#include "Texture.h"

namespace SpaRcle {
	namespace Graphics {
		class ResourceManager;
		class Shader;
		class Mesh;
		class Material {
			friend class ResourceManager;
			friend class Mesh;
		protected:
			~Material() {

			}
			Material(bool transparent, Shader* shader, std::string name = "Unnamed", bool isDefault = false);
			Material(bool transparent, Shader* shader, std::vector<Texture*> textures, std::string name = "Unnamed", bool isDefault = false);
		protected:
			const bool	m_transparent		= false;
			bool		m_is_destroy		= false;
			const bool  m_isDefault			= false;
			const std::string m_name		= "";
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
			Texture* GetDiffuse()	const noexcept { return m_diffuse;		}
			Texture* GetNormal()	const noexcept { return m_normal;		}
			Texture* GetSpecular()	const noexcept { return m_specular;		}
			Texture* GetGlossines() const noexcept { return m_glossiness;	}
		public:
			bool IsDefault() const { return m_isDefault; }
			std::string GetName() const { return m_name; }
			const bool IsTransparent() const noexcept { return m_transparent; }
			virtual bool Use();
		};
	}
}


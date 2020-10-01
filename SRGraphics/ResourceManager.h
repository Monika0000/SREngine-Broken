#pragma once
#include <vector>
#include <string>
#include <map>

#include "Skybox.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"

#include "TextureLoader.h"
#include "FbxLoader.h"
#include "ObjLoader.h"

#include <SRString.h>
#include <Debug.h>

namespace SpaRcle {
	using namespace Helper;

	namespace Graphics {
		class ResourceManager {
		private:
			ResourceManager() {};
			~ResourceManager() {};
		private:
			inline static bool				m_isInitialize					= false;
			inline static std::string		m_resource_path					= "";
			inline static Shader*			m_standart_geometry_shader		= nullptr;
		public:
			static bool Init(std::string resource_path) {
				if (m_isInitialize) {
					Debug::Error("ResourceManager::Init() : resource manager already initialized!");
					return false;
				}

				Debug::Info("Initializing resource manager...");

				m_isInitialize = true;
				m_resource_path = SRString::MakePath(resource_path);

				Debug::System("ResourceManager::Init() : Set resource folder : " + m_resource_path);

				return true;
			}
			static bool Destroy() {
				if (!m_isInitialize) {
					Debug::Error("ResourceManager::Destroy() : resource manager is not initialized!");
					return false;
				}

				Debug::Info("Destroying resource manager...");

				return true;
			}
			static std::string GetResourceFolder() noexcept {
				if (!m_isInitialize) {
					Debug::Error("ResourceManager::GetResourceFolder() : resource manager is not initialized!");
					return "";
				} else return m_resource_path;
			}

			static bool SetStandartGeometryShader(Shader* shader) {
				if (!ResourceManager::m_isInitialize) {
					Debug::Error("ResourceManager::SetStandartGeometryShader() : resource manager is not initialize!");
					return false;
				}
				else {
					if (ResourceManager::m_standart_geometry_shader) {
						Debug::Error("ResourceManager::SetStandartGeometryShader() : shader already set!");
						return false;
					}
					else {
						Debug::Info("ResourceManager::SetStandartGeometryShader() : setting shader...");
						ResourceManager::m_standart_geometry_shader = shader;
						return true;
					}
				}

				return false;
			}
			static Material* GetDefaultMaterial() {
				if (!ResourceManager::m_isInitialize) {
					Debug::Error("ResourceManager::GetDefaultMaterial() : resource manager is not initialize!");
					return nullptr;
				}

				static Material* def_mat = nullptr;
				if (!def_mat)
					def_mat = new Material(false);
				return def_mat;
			}
			static Shader* GetStandartGeometryShader() {
				if (!ResourceManager::m_isInitialize) {
					Debug::Error("ResourceManager::GetStandartGeometryShader() : resource manager is not initialize!");
					return nullptr;
				}

				if (ResourceManager::m_standart_geometry_shader)
					return m_standart_geometry_shader;
				else {
					Debug::Error("ResourceManager::GetStandartGeometryShader() : shader is nullptr!");
					return nullptr;
				}
			}
		public:
			static std::vector<Mesh*> LoadObjModel(std::string name) {
				std::string path = SRString::MakePath(ResourceManager::m_resource_path + "\\Models\\" + name);
				return ObjLoader::Load(path);
			}
			static std::vector<Mesh*> LoadFbxModel(std::string name) { }
			//====================================================== 
			static Material*	LoadMaterial(std::string name) { }
			static Skybox*		LoadSkybox(std::string name) { }
		};
	}
}


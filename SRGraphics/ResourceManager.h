#pragma once
#include <vector>
#include <string>
#include <map>

#include "Skybox.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"

#include <Debug.h>

namespace SpaRcle {
	using namespace Helper;

	namespace Graphics {
		class ResourceManager {
		private:
			ResourceManager() {};
			~ResourceManager() {};
		private:
			bool			m_isInitialize			= false;
			std::string		m_resource_path			= "";
		public:
			bool Init(std::string resource_path) {
				if (m_isInitialize) {
					Debug::Error("ResourceManager::Init() : resource manager already initialize!");
					return false;
				}

				Debug::Info("Initializing resource manager...");

				this->m_isInitialize = true;
				this->m_resource_path = resource_path;
			}
		public:
			static ResourceManager* Get() {
				static ResourceManager* resManager = nullptr;
				if (!resManager)
					resManager = new ResourceManager();
				return resManager;
			}
		public:
			static std::vector<Mesh*> LoadObjModel(std::string name) {}
			static std::vector<Mesh*> LoadFbxModel(std::string name){}
			//======================================================
			static Material*	LoadMaterial(std::string name){}
			static Skybox*		LoadSkybox(std::string name){}
		};
	}
}


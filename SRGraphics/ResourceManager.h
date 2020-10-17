#pragma once
#include <vector>
#include <string>
#include <map>

#include "Skybox.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"

#include "FbxLoader.h"
#include "ObjLoader.h"

#include <SRString.h>
#include <Debug.h>
#include "Video.h"

namespace SpaRcle {
	using namespace Helper;

	namespace Graphics {
		class ResourceManager {
		private:
			ResourceManager() {};
			~ResourceManager() {};
		private:
			inline static bool				m_isInitialize								= false;
			inline static std::string		m_resource_path								= "";
			inline static Shader*			m_standart_shader							= nullptr;

			inline static std::map<std::string, Material*>		m_materials				= {};
			inline static std::map<std::string, Video*>			m_videos				= {};
			inline static std::map<std::string, Mesh*>			m_meshes				= {};
			inline static std::map<std::string, Texture*>		m_textures				= {};
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

				Debug::Info("Destroying resource manager...\n\tMaterials : "+std::to_string(m_materials.size()) +
					"\n\tVideos : " + std::to_string(m_videos.size())+
					"\n\tMeshes : " + std::to_string(m_meshes.size()) +
					"\n\tTextures : " + std::to_string(m_textures.size()));

				for (auto a : m_materials) {
					a.second->Destroy();
					delete a.second;
				}
				for (auto a : m_videos) {
					a.second->Destroy();
					delete a.second;
				}
				for (auto a : m_meshes) {
					a.second->Destroy();
					delete a.second;
				}	

				for (auto a : m_textures) {
					a.second->Destroy();
					delete a.second;
				}

				return true;
			}
			static std::string GetResourceFolder() noexcept {
				if (!m_isInitialize) {
					Debug::Error("ResourceManager::GetResourceFolder() : resource manager is not initialized!");
					return "";
				} else return m_resource_path;
			}

			static bool SetStandartShader(Shader* shader) {
				if (!ResourceManager::m_isInitialize) {
					Debug::Error("ResourceManager::SetStandartShader() : resource manager is not initialize!");
					return false;
				}
				else {
					if (ResourceManager::m_standart_shader) {
						Debug::Error("ResourceManager::SetStandartShader() : shader already set!");
						return false;
					}
					else {
						Debug::Info("ResourceManager::SetStandartShader() : setting shader...");
						ResourceManager::m_standart_shader = shader;
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
				if (!def_mat) {
					Texture* def_tex = ResourceManager::LoadTexture(
						"default.png",
						Texture::Type::Diffuse,
						Texture::Filter::NEAREST
					);
					def_mat = CreateMaterial(false, {def_tex});
				}
				return def_mat;
			}
			static Shader* GetStandartShader() {
				if (!ResourceManager::m_isInitialize) {
					Debug::Error("ResourceManager::GetStandartShader() : resource manager is not initialize!");
					return nullptr;
				}

				if (ResourceManager::m_standart_shader)
					return m_standart_shader;
				else {
					Debug::Error("ResourceManager::GetStandartShader() : shader is nullptr!");
					return nullptr;
				}
			}
		public:
			static void Destroy(Material* material) {

			}
			static void Destroy(Video* video) {

			}

			static Material* CreateMaterial(bool transparent = false, std::vector<Texture*> textures = {}) {
				static int counter = 0;
				counter++;
				Material* mat = new Material(transparent, GetStandartShader(), textures);
				m_materials.insert(std::make_pair("ResourceManager::CreateMaterial() - "+std::to_string(counter), mat));
				return mat;
			}
			static Video* LoadVideo(std::string file_name, Video::PlayMode playMode, Video::RenderMode renderMode = Video::RenderMode::CalculateInRealTime) {
				file_name = m_resource_path + "\\Videos\\" + file_name;
				file_name = SRString::MakePath(file_name);

				auto find = m_videos.find(file_name);
				if (find != m_videos.end())
					return find->second;

				Debug::Log("ResourceManager::LoadVideo() : loading \"" + file_name + "\" video...");
				Video* vid = new Video(file_name, playMode, renderMode);
				m_videos.insert(std::make_pair(file_name, vid));
				return vid;
			}
		public:
			static std::map<std::string, Video*>* GetVidesBuffer() {
				return &ResourceManager::m_videos;
			}
		public:
			static std::vector<Mesh*> LoadObjModel(std::string name) {
				int counter = 0;
				std::string path = SRString::MakePath(ResourceManager::m_resource_path + "\\Models\\" + name + ".obj");

				std::vector<Mesh*> meshes = ObjLoader::Load(path);

				for (auto a : meshes) {
					m_meshes.insert(std::make_pair(path + " - " + std::to_string(counter), a));
					counter++;
				}
				return meshes;
			}
			static std::vector<Mesh*> LoadFbxModel(std::string name) { }
			//====================================================== 
			static Texture*		LoadTexture(std::string name, Texture::Type type = Texture::Type::Diffuse, Texture::Filter filter = Texture::Filter::NEAREST);
			static Material*	LoadMaterial(std::string name) { }
			static Skybox*		LoadSkybox(std::string name) { }
		};
	}
}


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

#include <tuple>

#include <SRFile.h>

namespace SpaRcle {
	using namespace Helper;

	namespace Graphics {
		class GameObject;

		class ResourceManager {
			friend class GameObject;
		private:
			ResourceManager() {};
			~ResourceManager() {};
		private:
			inline static volatile bool								m_isDebug									= false;

			inline static bool										m_isInitialize								= false;
			inline static std::string								m_absolute_resource_path					= "";
			inline static std::string								m_local_resource_path						= "";
			inline static Shader*									m_standart_shader							= nullptr;
			inline static Shader*									m_skybox_shader								= nullptr;

			inline static std::vector<GameObject*>					m_gameObjects								= {};
			inline static std::map<std::string, Material*>			m_materials									= {};
			inline static std::map<std::string, Video*>				m_videos									= {};
			//inline static std::map<std::string, Mesh*>				m_meshes									= {};
			inline static std::vector<Mesh*>						m_meshes									= {};
			inline static std::map<std::string, Texture*>			m_textures									= {};

			inline static bool										m_destroy_video								= false;
		public:
			static Mesh* FindMesh(std::string file_name);
		public:
			static bool Init(std::string resource_path = "");
			static bool Destroy() {
				if (!m_isInitialize) {
					Debug::Error("ResourceManager::Destroy() : resource manager is not initialized!");
					return false;
				}

				Debug::Info("Destroying resource manager...\n\tMaterials : "+std::to_string(m_materials.size()) +
					"\n\tVideos : " + std::to_string(m_videos.size())+
					"\n\tMeshes : " + std::to_string(m_meshes.size()) +
					"\n\tTextures : " + std::to_string(m_textures.size())+
					"\n\tGameObjects : " + std::to_string(m_gameObjects.size())
				);

				for (auto a : m_materials) {
					a.second->Destroy();
					delete a.second;
				}
				for (auto a : m_videos) {
					a.second->Destroy();
					delete a.second;
				}
				for (auto a : m_meshes) {
					a->Destroy();
					//a.second->Destroy();
					delete a;
					//delete a.second;
				}	

				for (auto a : m_textures) {
					a.second->Destroy();
					delete a.second;
				}

				return true;
			}
			static std::string GetLocalResourceFolder() {
				if (!m_isInitialize) {
					Debug::Error("ResourceManager::GetLocalResourceFolder() : resource manager is not initialized!");
					return "";
				}
				else { 
					if (m_local_resource_path == "") {
						Debug::Error("ResourceManager::GetLocalResourceFolder() : TODO! Local path is not set!"); //TODO:!
						return "";
					}
					else return m_local_resource_path;
				}
			}
			static std::string GetAbsoluteResourceFolder() noexcept {
				if (!m_isInitialize) {
					Debug::Error("ResourceManager::GetAbsoluteResourceFolder() : resource manager is not initialized!");
					return "";
				} else return m_absolute_resource_path;
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

			/*
			static bool SetSkyboxShader(Shader* shader) {
				if (!ResourceManager::m_isInitialize) {
					Debug::Error("ResourceManager::SetSkyboxShader() : resource manager is not initialize!");
					return false;
				}
				else {
					if (ResourceManager::m_skybox_shader) {
						Debug::Error("ResourceManager::SetSkyboxShader() : shader already set!");
						return false;
					}
					else {
						Debug::Info("ResourceManager::SetSkyboxShader() : setting shader...");
						ResourceManager::m_skybox_shader = shader;
						return true;
					}
				}

				return false;
			}
			static Shader* GetSkyboxShader() {
				if (!ResourceManager::m_isInitialize) {
					Debug::Error("ResourceManager::GetSkyboxShader() : resource manager is not initialize!");
					return nullptr;
				}

				if (ResourceManager::m_skybox_shader)
					return m_skybox_shader;
				else {
					Debug::Error("ResourceManager::GetSkyboxShader() : shader is nullptr!");
					return nullptr;
				}
			}*/
		public:
			static void Destroy(Mesh* mesh) {
				Debug::Log("ResourceManager::Destroy() : Destroying \""+mesh->m_name+"\" mesh...");
				/*for (auto& a : m_meshes)
				{
					if (a.second == mesh) {
						m_meshes.erase(a.first);
						break;
					}
				}*/
				for (size_t t = 0; t < m_meshes.size(); t++) {
					if (m_meshes[t] == mesh) {
						m_meshes.erase(m_meshes.begin() + t);
						break;
					}
				}
				mesh->Destroy();
				delete mesh;
			}
			static void Destroy(Material* material) {

			}
			static void Destroy(Video* video) {
				Debug::Log("ResourceManager::Destroy() : destroying "+video->GetFileName()+" video...");
				m_destroy_video = true;

				m_videos.erase(video->GetFileName());

				Sleep(25); // Ждем пока все потоки "осознают", что данная структура будет уничтожена

				video->Destroy();
				// NOT!!! delete video; See mesh
				video = nullptr;

				m_destroy_video = false;
			}

			static Material* CreateMaterial(bool transparent = false, std::vector<Texture*> textures = {}) {
				static int counter = 0;
				counter++;
				Material* mat = new Material(transparent, GetStandartShader(), textures);
				m_materials.insert(std::make_pair("ResourceManager::CreateMaterial() - "+std::to_string(counter), mat));
				return mat;
			}
			static Video* LoadVideo(std::string file_name, Video::PlayMode playMode, Video::RenderMode renderMode = Video::RenderMode::CalculateInRealTime) {
				file_name = m_absolute_resource_path + "\\Videos\\" + file_name;
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
			static std::vector<GameObject*> GetGameObjects() { return ResourceManager::m_gameObjects; }
			static std::map<std::string, Video*> GetVideosBuffer() {
			ret: if (m_destroy_video) goto ret;
				return ResourceManager::m_videos;
			}
		public:
			static std::vector<Mesh*> LoadObjModel(std::string name);
			static std::vector<Mesh*> LoadFbxModel(std::string name) { }
			//====================================================== 
			/*
				if isAbsPath == true, then will use full dir to file, else, will use resource path

			*/
			static Texture*		LoadTexture(std::string name, Texture::Type type = Texture::Type::Diffuse, Texture::Filter filter = Texture::Filter::NEAREST, bool isAbsPath = false);

			static GameObject*  LoadPrefab(std::string file_name, std::string gm_name = "Random name");
			static Material*	LoadMaterial(std::string name) { }
			static Skybox*		LoadSkybox(std::string name, std::string img_format = ".jpg");
		};
	}
}


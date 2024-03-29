#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "Shader.h"
#include "Mesh.h"
#include "Skybox.h"
#include "Camera.h"

#include <Debug.h>

#include "Animator.h"

#include <functional>

#include "ResourceManager.h"

namespace SpaRcle {
	using namespace Helper;

	namespace Graphics {
		class Window;

		class Render {
		public:
			Render(Skybox* skybox, Camera* camera) {
				this->m_skybox = skybox;
				this->m_camera = camera;
			};
			~Render() {};
		private:
			std::map<std::string, std::function<void(void)>>		m_gui_elements						= std::map<std::string, std::function<void(void)>>();
			Skybox*													m_skybox							= nullptr;
			Window*													m_window							= nullptr;
			Camera*													m_camera							= nullptr;
		private:
			std::vector<Animator*>									m_animators							= std::vector<Animator*>();
			//!=================================================================================================================================================
			std::vector<Mesh*>										m_transparent_meshes				= std::vector<Mesh*>();
			unsigned int											m_count_transparent_meshes			= 0;
			//!=================================================================================================================================================
			std::vector<Mesh*>										m_meshes							= std::vector<Mesh*>();
			unsigned int											m_count_meshes						= 0;

			//!=================================================================================================================================================
			bool													m_has_meshes_to_remove				= false;
			bool													m_removing_meshes_now				= false;
			std::vector<Mesh*>										m_meshes_to_remove					= {};

			bool													m_has_materials_to_remove			= false;
			bool													m_removing_materials_now			= false;
			std::vector<Material*>									m_materials_to_remove				= {};
			//!=================================================================================================================================================

			bool													m_calculate_meshes_now				= false;
			bool													m_has_copy_meshes_to_calc			= false;
			std::vector<Mesh*>										m_copy_meshes_calculate				= {};
		private:
			size_t													m_t									= 0;
		private:
			Shader*													m_selector_shader					= nullptr;
			Shader*													m_skybox_shader						= nullptr;
			Shader*													m_geometry_shader					= nullptr;
			Shader*													m_post_processing					= nullptr;
			Shader*													m_grid_shader						= nullptr;
		private:
			bool													m_isCreated							= false;
			bool													m_isInit							= false;
			bool													m_isRunning							= false;

			bool													m_is_draw_now						= false;

			/*
				 0	- not finding now
				-1	- not found mesh
				 1	- mesh has been found
				 2	- finding now
			*/
			int													    m_find_aimed_mesh_stat				= 0;
			Mesh*													m_current_aimed_mesh				= nullptr;
		private:
			void GC();
			void SortTransparentMeshes();
		public:
			void AddMeshToCaclulate(Mesh* mesh);
			void RemoveMaterial(Material* mat) {
			ret: if (m_removing_materials_now) goto ret;
				if (Debug::GetLevel() >= Debug::Level::Hight)
					Debug::Log("Render::RemoveMaterial() : register \"" + mat->GetName() + "\" material to remove.");
				m_materials_to_remove.push_back(mat);
				m_has_materials_to_remove = true;
			}
			void RemoveMesh(Mesh* mesh) {
			ret: if (m_removing_meshes_now) goto ret;
				if (Debug::GetLevel() >= Debug::Level::Hight)
					Debug::Log("Render::RemoveMesh() : register \""+mesh->m_name+"\" mesh to remove.");
				m_meshes_to_remove.push_back(mesh);
				m_has_meshes_to_remove = true;
			}
			void AddMesh(Mesh* meshe);
			void AddMeshes(std::vector<Mesh*>& meshes);
			std::vector<Mesh*>& GetMeshes() noexcept { return m_meshes; }
			Window* GetWindow();
			bool AddGUI(std::string key_name, std::function<void(void)> func) {
				auto find = this->m_gui_elements.find(key_name);
				if (find == m_gui_elements.end()) {
					m_gui_elements.insert(std::make_pair(key_name, func));
					return true;
				}
				else 
					return false;
			}
		public:
			/* call from random location */
			Mesh* GetAimedMesh();
		public:
			bool Create(Window* window);

			/* Call only from window thread */
			bool Init();
			/* Call only from window thread */
			bool Run();
			/* Call only from window thread */
			bool Close();
		public:
			void PoolEvents();
			void FindAimedMesh();
			void DrawGrid();
			void DrawSkybox();
			void PlayAnimators();
			void DrawGeometry();
			void DrawGUI();
		};
	}
}


#pragma once
#include "Shader.h"
#include "Mesh.h"
#include "Skybox.h"
#include "Canvas.h"
#include "Camera.h"

namespace SpaRcle {
	namespace Graphics {
		class Window;

		class Render {
		public:
			Render(Skybox* skybox) {
				this->m_skybox = skybox;
			};
			~Render() {};
		private:
			Skybox*				m_skybox				= nullptr;
			Canvas*				m_canvas				= nullptr;
			Window*				m_window				= nullptr;
		private:
			std::vector<Mesh*>	m_meshes				= std::vector<Mesh*>();
			unsigned int		m_count_meshes			= 0;
		private:
			Shader*				m_skybox_shader			= nullptr;
			Shader*				m_geometry_shader		= nullptr;
			Shader*				m_post_processing		= nullptr;
		private:
			bool				m_isCreated				= false;
			bool				m_isInit				= false;
			bool				m_isRunning				= false;
		private:
			void SortMeshes();
		public:
			void AddMeshes(std::vector<Mesh*>& meshes);
			std::vector<Mesh*>& GetMeshes() noexcept { return m_meshes; }
			Window* GetWindow();
		public:
			bool Create(Window* window);
			bool Init();
			bool Run();
			bool Close();
		public:
			void DrawSkybox();
			void DrawGeometry();
			void DrawGUI();
		};
	}
}


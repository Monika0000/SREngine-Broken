#pragma once
#include "Shader.h"
#include "Mesh.h"
#include "Skybox.h"
#include "Canvas.h"
#include "Camera.h"

namespace SpaRcle {
	namespace Graphics {
		class Render {
		public:
			Render(Skybox* skybox) {
				this->m_skybox = skybox;
			};
			~Render() {};
		private:
			Skybox*				m_skybox				= nullptr;
			Canvas*				m_canvas				= nullptr;
		private:
			std::vector<Mesh*>	m_meshes				= std::vector<Mesh*>();
		private:
			Shader*				m_skybox_shader			= nullptr;
			Shader*				m_geometry_shader		= nullptr;
			Shader*				m_post_processing		= nullptr;
		private:
			bool				m_isCreated				= false;
			bool				m_isInit				= false;
			bool				m_isRunning				= false;
		public:
			bool Create();
			bool Init();
			bool Run();
			void Close();
		public:
			void DrawSkybox();
			void DrawGeometry();
			void DrawGUI();
		};
	}
}


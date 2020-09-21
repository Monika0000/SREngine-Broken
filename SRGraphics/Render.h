#pragma once
#include "Shader.h"
#include "Mesh.h"
#include "Skybox.h"
#include "Canvas.h"
#include "Camera.h"

namespace SpaRcle {
	namespace Graphics {
		class Render {
		private:
			Skybox*				m_skybox				= nullptr;
		private:
			std::vector<Mesh*>	m_meshes				= std::vector<Mesh*>();
		private:
			Shader*				m_skybox_shader			= nullptr;
			Shader*				m_geometry_shader		= nullptr;
			Shader*				m_post_processing		= nullptr;
		public:
			void DrawSkybox();
			void DrawGeometry();
			void DrawGUI();
		};
	}
}


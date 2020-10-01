#pragma once
#define GLEW_STATIC
#define NOMINMAX

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <vector>
#include "Material.h"
#include "Shader.h"
#include "Skeleton.h"

namespace SpaRcle {
	namespace Graphics {
		class Camera;

		struct Vertex {
			glm::vec3 position;
			glm::vec2 texCoords;
			glm::vec3 normal;
			glm::vec3 tangent;
			int boneID1;
			int boneID2;
			int boneID3;
			int boneID4;
		};

		class Mesh {
		public:
			Mesh(Shader* geometry_shader, Material* material);
			~Mesh() {
				this->m_geometry_shader = nullptr;
				this->m_material		= nullptr;
			}
		private:
			size_t					m_count_vertices		= 0;
			std::vector<Vertex>		m_vertexes			= std::vector<Vertex>();
		private:
			GLuint					m_VAO				= 0;
			GLuint					m_VBO				= 0;
		private:
			bool					m_is_destroyed		= false;
			bool					m_is_calculated		= false;
			Shader*					m_geometry_shader	= nullptr;
			Camera*					m_camera			= nullptr;
		private:
			Material*				m_material			= nullptr;
			Skeleton*				m_skeleton			= nullptr;
		public:
			glm::vec3				m_position			= glm::vec3();
			glm::vec3				m_rotation			= glm::vec3();
			glm::vec3				m_scale				= glm::vec3();
		public:
			Material* GetMaterial() { return m_material; }
		public:
			void SetVertexArray(std::vector<Vertex>& vertexes) noexcept;
			bool Destroy() noexcept;
			bool Draw();
			bool Calculate();
		};
	}
}

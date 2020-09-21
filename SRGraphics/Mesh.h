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
		struct Vertex {
			glm::vec3 position;
			glm::vec2 texCoord;
			glm::vec3 normal;
			glm::vec3 tangent;
			int boneID1;
			int boneID2;
			int boneID3;
			int boneID4;
		};

		class Mesh {
		public:
			Mesh(Shader* geometry_shader, Material* material) {
				this->m_geometry_shader = geometry_shader;
				this->m_material		= material;
			}
			~Mesh() {
				this->m_geometry_shader = nullptr;
				this->m_material		= nullptr;
			}
		private:
			bool		m_is_destroyed		= false;
			bool		m_is_calculated		= false;
			Shader*		m_geometry_shader	= nullptr;
		public:
			Material*	m_material			= nullptr;
			Skeleton*	m_skeleton			= nullptr;
		public:
			glm::vec3	m_position			= glm::vec3();
			glm::vec3	m_rotation			= glm::vec3();
			glm::vec3	m_scale				= glm::vec3();
		public:
			bool Destroy() noexcept;
			bool Draw();
			bool Calculate();
		};
	}
}

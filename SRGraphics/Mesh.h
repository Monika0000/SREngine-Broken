#pragma once
#define GLEW_STATIC
#define NOMINMAX

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <map>

#include <iostream>
#include <vector>
#include "Material.h"
#include "Shader.h"
#include "Skeleton.h"

#include "Component.h"

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

		class ResourceManager;
		class FbxLoader;
		class Render;
		class ObjLoader;
		class Mesh : public Component {
			friend class ResourceManager;
			friend class ObjLoader;
			friend class Render;
		private:
			inline static std::map<GLuint, unsigned long> VAO_Register_Buffer = {};
		private:
			Mesh(Shader* geometry_shader, Material* material, std::string name = "unnamed");

			/*
				call only resource manager
			*/
			~Mesh() {
				this->m_geometry_shader = nullptr;
				this->m_material = nullptr;
			}
		private:
			std::string				m_name				= "";
			std::string				m_file_name			= "";
			size_t					m_count_vertices	= 0;
			std::vector<Vertex>		m_vertexes			= std::vector<Vertex>();
		private:
			GLuint					m_VAO				= 0;
			//GLuint					m_VBO				= 0;
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
			glm::vec3				m_scale				= {1,1,1};
			glm::mat4				m_modelMat			= glm::mat4(0);
		private:
			/*
				call only OpenGL context
			*/
			bool FreeOpenGLMemory();
		private:
			void OnMoved(glm::vec3 new_val) override;
			void OnRotated(glm::vec3 new_val) override;
			void OnScaled(glm::vec3 new_val) override;
			void ReCalcModel();
		public:
			const char* TypeName() override { return "Mesh"; }
		public:
			Mesh* Copy(bool copy_transform = true);
		public:
			Material* GetMaterial() { return m_material; }
			void SetMaterial(Material* material) {
				this->m_material = material;
			}
		public:
			void SetVertexArray(std::vector<Vertex>& vertexes) noexcept;

			/*
				call only resource manager
			*/
			bool Destroy() noexcept;

			bool Draw();
			bool Calculate();
		};
	}
}

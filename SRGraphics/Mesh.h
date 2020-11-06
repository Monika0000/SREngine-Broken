#pragma once
#define GLEW_STATIC
#define NOMINMAX

//#define SRE_USE_DRAW_ELEMENTS

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

#include <Debug.h>

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
			std::string						m_name				= "";
			std::string						m_file_name			= "";
			size_t							m_count_vertices	= 0;
			//size_t							m_count_tex_coords	= 0;
			//size_t							m_count_indices		= 0;

#ifndef SRE_USE_DRAW_ELEMENTS
			std::vector<Vertex>				m_vertexes = std::vector<Vertex>();
#else
			std::vector<float>				m_vertices   = std::vector<float>();
#endif // !SRE_USE_DRAW_ELEMENTS

			//std::vector<float>				m_tex_coords = std::vector<float>();
			//std::vector<unsigned int>		m_indices			= std::vector<unsigned int>();
		private:
			GLuint							m_VAO				= 0;
			//GLuint							m_IBO				= 0;
			//GLuint						m_VBO				= 0;
		private:
			bool							m_is_destroyed		= false;
			bool							m_is_calculated		= false;
			Shader*							m_geometry_shader	= nullptr;
			Camera*							m_camera			= nullptr;
		private:
			Material*						m_material			= nullptr;
			Skeleton*						m_skeleton			= nullptr;
		public:
			glm::vec3						m_position			= glm::vec3();
			glm::vec3						m_rotation			= glm::vec3();
			glm::vec3						m_scale				= {1,1,1};
			glm::mat4						m_modelMat			= glm::mat4(0);
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
			//size_t GetCountIndices() const { return m_count_indices; }
			size_t GetCountVertices() const { return m_count_vertices; }
		public:
			const char* TypeName() override { return "Mesh"; }
		public:
			Mesh* Copy(bool copy_transform = true);
		public:
			std::string GetName() const { return m_name; }
			std::string GetFileName() const { return m_file_name; }
			Material* GetMaterial() { return m_material; }
			void SetMaterial(Material* material) {
				if (!material) {
					Helper::Debug::Error("Mesh::SetMaterial() : failed set nullptr material!");
				}
				else
					this->m_material = material;
			}
		public:
#ifndef SRE_USE_DRAW_ELEMENTS
			void SetVertexArray(std::vector<Vertex>& vertexes) noexcept {
				this->m_is_calculated = false;
				this->m_count_vertices = vertexes.size();
				this->m_vertexes = vertexes;
			}
#else
			void SetVertexArray(std::vector<float>& vertexes) noexcept {
				this->m_is_calculated = false;
				this->m_count_vertices = vertexes.size();
				this->m_vertices = vertexes;
			}
			void SetTextureCoordsArray(std::vector<float> & texs) noexcept {
					this->m_is_calculated = false;
					this->m_count_tex_coords = texs.size();
					this->m_tex_coords = texs;
			}
			void SetIndexArray(std::vector<unsigned int>& indices) {
				this->m_is_calculated = false;
				this->m_count_indices = indices.size();
				this->m_indices = indices;
			}
#endif // !SRE_USE_DRAW_ELEMENTS

			/*
				call only resource manager
			*/
			bool Destroy() noexcept;

			void FlatDraw(unsigned int color_id);
			bool Draw();
			bool Calculate();
		};
	}
}

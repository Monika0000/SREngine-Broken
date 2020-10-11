#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>

namespace SpaRcle {
	namespace Graphics {
		class Component;
		class GameObject;

		class Transform {
		public:
			Transform(GameObject* gameObject);
			Transform(GameObject* gameObject, glm::vec3 pos, glm::vec3 rot, glm::vec3 scl);
		private:
			GameObject*					m_gameObject	= nullptr;
			std::vector<Component*>*	m_gm_components = nullptr;
		private:
			glm::vec3 m_position	= { 0, 0, 0 };
			glm::vec3 m_rotation	= { 0, 0, 0 };
			glm::vec3 m_scale		= { 0, 0, 0 };
		public:
			void Translate(glm::vec3 translation,		bool local = false);
			void Rotate(glm::vec3 eulerAngles,			bool local = false);
			void Scaling(glm::vec3 scale,				bool local = false);

			void Translate(float x, float y, float z,	bool local = false);
			void Rotate(float x, float y, float z,		bool local = false);
			void Scaling(float x, float y, float z,		bool local = false);
		};
	}
}


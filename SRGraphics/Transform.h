#pragma once
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
			GameObject* m_gameObject = nullptr;
		private:
			glm::vec3 m_position;
			glm::vec3 m_rotation;
			glm::vec3 m_scale;
		public:
			void Translate(glm::vec3 translation);
			void Rotate(glm::vec3 eulerAngles);
			void Scaling(glm::vec3 scale);
		};
	}
}


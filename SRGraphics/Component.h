#pragma once
#include <glm/glm.hpp>

namespace SpaRcle {
	namespace Graphics {
		class GameObject;
		class Transform;

		class Component {
			friend class Transform;
		protected:
			virtual void OnMoved(glm::vec3 new_pos) { };
			virtual void OnRotated(glm::vec3 new_rot) { };
			virtual void OnScaled(glm::vec3 new_rot) { };
		public:
			Component() {};
			~Component() {};
		public:
			GameObject* GetGameObject() { return m_gameObject; }
		private:
			GameObject* m_gameObject = nullptr;
		};
	}
}


#pragma once
#include <glm/glm.hpp>

namespace SpaRcle {
	namespace Graphics {
		class GameObject;
		class Transform;

		class Component {
			friend class Transform;
			friend class GameObject;
		public:
			bool Enabled() const { return m_enable; }
			virtual void Enable(bool enable) { };
			virtual const char* TypeName() { return "BaseComponent"; }
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
			bool			m_enable			= true;
			GameObject*		m_gameObject		= nullptr;
		};
	}
}


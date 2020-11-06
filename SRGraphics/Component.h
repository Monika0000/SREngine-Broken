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
			virtual void Enable(bool enable) { m_enable = enable; };
			virtual const char* TypeName() { return "BaseComponent"; }
		protected:
			bool			m_enable = true;
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
			GameObject*		m_gameObject		= nullptr;
		};
	}
}


#pragma once
#include <string>
#include <vector>

namespace SpaRcle {
	namespace Graphics {
		class Component;
		class Transform;

		class GameObject {
			friend class Transform;
		private:
			GameObject(std::string name);
			~GameObject();
		public:
			Transform* GetTransform() const noexcept { return m_transform; };
		private:
			Transform*					m_transform		= nullptr;
		private:
			std::string					m_name			= "";
			std::vector<Component*>		m_components	= std::vector<Component*>();
		public:
			bool AddComponent(Component* component);
		public:
			static bool Destroy(GameObject* gameObject);
			static GameObject* Instance(std::string name);
		};
	}
}


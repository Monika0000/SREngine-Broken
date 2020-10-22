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
			std::vector<GameObject*>	m_childrens			= { };
			size_t						m_count_childrens	= 0;
			GameObject*					m_parent			= nullptr;
		private:
			Transform*					m_transform			= nullptr;
		private:
			std::string					m_name				= "";
			std::string					m_tag				= "Untagged";
			std::vector<Component*>		m_components		= std::vector<Component*>();
		public:
			bool HasChildrens() const { return m_count_childrens != 0; }
			std::vector<GameObject*> GetChilderns() const { return m_childrens; }
			std::string GetName() const { return m_name; }
			std::string GetTag() const { return m_tag; }
		public:
			template <typename T> bool AddComponents(std::vector<T> components) {
				for (auto a : components)
					if (!AddComponent(a))
						return false;
				return true;
			}
			bool AddComponent(Component* component);
		public:
			static std::vector<GameObject*> Find(std::string name);
			static std::vector<GameObject*> FindOfTag(std::string tag);
			static bool Destroy(GameObject* gameObject);
			static bool Destroy(std::string name);
			static GameObject* Instance(std::string name);
		};
	}
}


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
			GameObject(std::string name, const bool editor_object = false);
			~GameObject();
		public:
			Transform* GetTransform() const noexcept { return m_transform; };
		private:
			const bool					m_editor_object		= false;
			std::vector<GameObject*>	m_childrens			= { };
			size_t						m_count_childrens	= 0;
			GameObject*					m_parent			= nullptr;
		private:
			Transform*					m_transform			= nullptr;
		private:
			bool						m_is_enabled		= true;
			bool						m_is_select			= false;
			std::string					m_name				= "";
			std::string					m_tag				= "Untagged";
			std::vector<Component*>		m_components		= std::vector<Component*>();
		public:
			bool Enabled() const { return m_is_enabled; }
			void Enable(bool v);
		public:
			std::vector<Component*> GetComponents() const { return m_components; }
			void InvertSelect();
			bool IsSelect() const { return m_is_select; }
			void SetParent(GameObject* parent) {
				this->m_parent = parent;
			}
			bool IsChildren() const { return (bool)m_parent; }
			bool HasChildrens() const { return m_count_childrens != 0; }
			std::vector<GameObject*> GetChilderns() const { return m_childrens; }
			std::string GetName() const { return m_name; }
			void SetName(std::string name) { this->m_name = name; }
			std::string GetTag() const { return m_tag; }
			void AddChild(GameObject* child) {
				child->SetParent(this);
				this->m_childrens.push_back(child);
				m_count_childrens++;
			}
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
			static GameObject* Instance(std::string name, const bool editor = false);
		};
	}
}


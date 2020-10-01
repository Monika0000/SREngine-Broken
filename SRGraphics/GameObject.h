#pragma once
#include <string>

namespace SpaRcle {
	namespace Graphics {
		class Component;

		class GameObject {
		private:
			GameObject() {};
			~GameObject() {

			}
		private:
			std::string m_name = "";
		private:
			bool AddComponent(Component* component);
		public:
			static bool Destroy(GameObject* gameObject);
			static GameObject* Instance(std::string name);
		};
	}
}


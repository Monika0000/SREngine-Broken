#pragma once
#include <string>


namespace SpaRcle {
	namespace Graphics {
		class Scene {
		private:
			Scene() { };
			~Scene() { };
		private:
			inline static std::string m_current_scene = "Default scene";
		public:
			static std::string GetName() { return m_current_scene; }
		public:
			static bool Load(std::string name);
			static bool Close();
			static bool Create(std::string name);
			static bool Save();
		};
	}
}
#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>

#include <SRString.h>

namespace SpaRcle {
	using namespace Helper;
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
			glm::vec3 m_scale		= { 1, 1, 1 };
		public:
			void SetPosition(glm::vec3 pos);
			void SetRotation(glm::vec3 rot);
			void SetScale(glm::vec3 scl);

			void SetPosition(float x, float y, float z);
			void SetRotation(float x, float y, float z);
			void SetScale(float x, float y, float z);
		private:
			const int m_count_symbols_string = 100000;
		public:
			std::string GetStringPosition() const {
				return
					  SRString::Remove(std::to_string(m_position.x), (size_t)m_count_symbols_string) + ", "
					+ SRString::Remove(std::to_string(m_position.y), (size_t)m_count_symbols_string) + ", "
					+ SRString::Remove(std::to_string(m_position.z), (size_t)m_count_symbols_string);
			}
			std::string GetStringRotation() const {
				return
					  SRString::Remove(std::to_string(m_rotation.x), (size_t)m_count_symbols_string) + ", "
					+ SRString::Remove(std::to_string(m_rotation.y), (size_t)m_count_symbols_string) + ", "
					+ SRString::Remove(std::to_string(m_rotation.z), (size_t)m_count_symbols_string);
			}
			std::string GetStringScale() const {
				return
					  SRString::Remove(std::to_string(m_scale.x),	 (size_t)m_count_symbols_string) + ", "
					+ SRString::Remove(std::to_string(m_scale.y),	 (size_t)m_count_symbols_string) + ", "
					+ SRString::Remove(std::to_string(m_scale.z),	 (size_t)m_count_symbols_string);
			}
		public:
			void Translate(glm::vec3 translation,		bool local = false);
			void Rotate(glm::vec3 eulerAngles,			bool local = false);
			void Scaling(glm::vec3 scale,				bool local = false, bool mul = false);

			void Translate(float x, float y, float z,	bool local = false);
			void Rotate(float x, float y, float z,		bool local = false);
			void Scaling(float x, float y, float z,		bool local = false);
		};
	}
}


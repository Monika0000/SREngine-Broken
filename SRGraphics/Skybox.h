#pragma once
#include "Texture.h"
#include "Shader.h"

namespace SpaRcle {
	namespace Graphics {
		class Skybox {
		private:
			Shader*  m_skybox_shader	= nullptr;
		private:
			Texture* m_back				= nullptr;
			Texture* m_bottom			= nullptr;
			Texture* m_front			= nullptr;
			Texture* m_left				= nullptr;
			Texture* m_right			= nullptr;
			Texture* m_top				= nullptr;
		public:
			Skybox(
				Shader* shader,
				Texture* back,
				Texture* bottom,
				Texture* front,
				Texture* left,
				Texture* right,
				Texture* top
			) : m_back(back), m_bottom(bottom), m_front(front), m_left(left), m_right(right), m_top(top) {
				this->m_skybox_shader = shader;
			}
			~Skybox() {
				delete m_back;
				delete m_bottom;
				delete m_front;
				delete m_left;
				delete m_right;
				delete m_top;
				this->m_skybox_shader = nullptr;
			}
		public:
			void Draw();
		};
	}
}


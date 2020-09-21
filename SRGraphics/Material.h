#pragma once
#include "Texture.h"

namespace SpaRcle {
	namespace Graphics {
		class Material {
		private:
			Texture* diffuse = nullptr;
			Texture* normal = nullptr;
			Texture* specular = nullptr;
			Texture* glossiness = nullptr;
		public:
			void Use();
		};
	}
}


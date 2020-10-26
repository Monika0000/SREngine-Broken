#pragma once
#include <string>
#include <Debug.h>
#include "GameObject.h"

namespace SpaRcle {
	namespace Graphics {
		class FbxLoader {
		public:
			static GameObject* Load(std::string file);
		};
	}
}


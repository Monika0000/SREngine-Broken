#pragma once

namespace SpaRcle {
	namespace Graphics {
		class Shader {
		public:
			bool Linking();
			bool Compile();
			bool Use();
			bool Destroy();
		};
	}
}


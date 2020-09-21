#pragma once
#include "Shader.h"

namespace SpaRcle {
	namespace Graphics {
		class Render {
		private:
			Shader* m_geometry_shader		= nullptr;
			Shader* m_post_processing		= nullptr;
			Shader* m_skybox_shader			= nullptr;
		};
	}
}


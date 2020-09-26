#pragma once
#include <string>
#include <malloc.h>
#include <vector>

#include "Mesh.h"

namespace SpaRcle {
	namespace Graphics {
		class ObjLoader {
		private:
			ObjLoader() {  };
			~ObjLoader() {  };
		private:
			inline static bool m_is_used			= false;
			inline static std::string m_file_name	= "";
		private:
			static bool ProcessLine(char* line);
			static std::vector<Mesh*> ProcessFile(const char* data);
		public:
			static std::vector<Mesh*> Load(std::string path);
		};
	}
}


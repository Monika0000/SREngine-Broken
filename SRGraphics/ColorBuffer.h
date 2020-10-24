#pragma once
#include <glm/glm.hpp>
#include <vector>

namespace SpaRcle {
	namespace Graphics {
		class ColorBuffer {
			struct cbInfo_t {
				int Name;
				glm::u8vec3 Color; // uint8 => 0 - 255 (unsigned char)
			};
		public:
			ColorBuffer() {

			}
			~ColorBuffer() { Names.clear(); }
		private:
			std::vector<cbInfo_t> Names = std::vector<cbInfo_t>();
			size_t next = 0;
		public:
			void InitNames(size_t count_models);
			void LoadName(size_t OBJECT_ID, glm::u8vec3 color);
			int GetSelectColorObject(unsigned char* pixel);
			bool CheckColor(glm::u8vec3& one, unsigned char* two);
		};
	}
}

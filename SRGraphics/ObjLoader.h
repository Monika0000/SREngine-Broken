#pragma once
#include <string>
#include <malloc.h>
#include <vector>

#include "Mesh.h"
#include <SRString.h>

namespace SpaRcle {
	using namespace Helper;

	namespace Graphics {
		class ObjLoader {
		private:
			ObjLoader() {  };
			~ObjLoader() {  };
		private:
			inline static glm::vec3 CalculateTangent() {

			}
		public:
			inline static glm::vec3 MakeVec3(const char* source, char chr, unsigned short start) {
				glm::vec3		position		= glm::vec3();
				unsigned char   found_floats	= 0;

				unsigned short	count			= 0;
				unsigned short	found			= start;

				static short t					= 0;
				static short len				= 0;

				len = SRString::FastStrLen(source);

				for (t = start; t < len; t++) {
					count++;

					if (t + 1 == len) {
						if (found_floats == 3)
							return position;

						char* temp = new char[count + 1]; strcpy(temp, "");

						strncat(temp, source + found, count);
						position[found_floats] = (float)atof(temp);
						found_floats++;

						delete[] temp;

						//std::cout << ">>" << temp << "<<" << std::endl;
						return position;
					}
					else if (source[t] == chr) {
						if (found_floats + 1 == 3)
							return position;

						char* temp = new char[count + 1]; strcpy(temp, "");

						strncat(temp, source + found, count - 1);
						position[found_floats] = (float)atof(temp);
						found_floats++;

						delete[] temp;
						//std::cout << ">>" << temp << "<<" << std::endl;

						found = t + 1;
						count = 0;
					}
				}
				return position;
			}
			inline static glm::vec2 MakeVec2(const char* source, char chr, unsigned short start) {
				glm::vec2		position = glm::vec3();
				unsigned char   found_floats = 0;

				unsigned short	count = 0;
				unsigned short	found = start;

				static short t = 0;
				static short len = 0;

				len = SRString::FastStrLen(source);

				for (t = start; t < len; t++) {
					count++;

					if (t + 1 == len) {
						if (found_floats == 2)
							return position;

						char* temp = new char[count + 1]; strcpy(temp, "");

						strncat(temp, source + found, count);
						position[found_floats] = (float)atof(temp);
						found_floats++;

						delete[] temp;

						//std::cout << ">>" << temp << "<<" << std::endl;
						return position;
					}
					else if (source[t] == chr) {
						if (found_floats == 2) //if (found_floats + 1 == 2) - bug
							return position;

						char* temp = new char[count + 1]; strcpy(temp, "");

						strncat(temp, source + found, count - 1);
						position[found_floats] = (float)atof(temp);
						found_floats++;

						delete[] temp;
						//std::cout << ">>" << temp << "<<" << std::endl;

						found = t + 1;
						count = 0;
					}
				}
				return position;
			}
		private:
			inline static unsigned long long			m_line_number		= 0;
			inline static std::string					m_current_object	= "";
			inline static std::vector<Mesh*>			m_temp_meshes		= std::vector<Mesh*>();
			inline static std::vector<Vertex>			m_temp_vertexes		= std::vector<Vertex>();

			inline static std::vector<glm::vec3>		m_pos_vertex		= std::vector<glm::vec3>();
			inline static std::vector<glm::vec2>		m_pos_texture		= std::vector<glm::vec2>();
			inline static std::vector<glm::vec3>		m_pos_normal		= std::vector<glm::vec3>();
		private:
			inline static bool							m_is_used			= false;
			inline static std::string					m_file_name			= "";
		private:
			static void AddMesh();
			static void Clear();
			static void ProcessFace(char** elems);
			static int ProcessLine(char* line);
			static void ProcessFile(const char* data);
		public:
			static std::vector<Mesh*> Load(std::string path);
		};
	}
}


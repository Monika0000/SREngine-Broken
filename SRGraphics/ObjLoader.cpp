#include "pch.h"
#include "ObjLoader.h"
#include <Debug.h>
#include <SRString.h>
#include "ResourceManager.h"

namespace SpaRcle {
	using namespace Helper;

	namespace Graphics {
		void ObjLoader::AddMesh() {
			if (m_temp_vertexes.size() > 0) {
				Mesh* mesh = new Mesh(ResourceManager::GetStandartShader(), nullptr);
				mesh->SetVertexArray(m_temp_vertexes);
				m_temp_meshes.push_back(mesh);
			}
			m_temp_vertexes.clear();
		}
		void ObjLoader::Clear() {
			m_current_object.clear();

			m_pos_vertex.clear();
			m_pos_texture.clear();
			m_pos_normal.clear();

			m_temp_vertexes.clear();

			m_line_number = 0;
		}
		void ObjLoader::ProcessFace(char** elems) {
			glm::vec3 face;
			glm::vec3 uv;
			glm::vec3 norm_index;

			unsigned char count = SRString::MathCount(elems[0], '/');

			float* indexes_1 = SRString::SplitFloats(elems[0], '/', 0, count + 1);
			float* indexes_2 = SRString::SplitFloats(elems[1], '/', 0, count + 1);
			float* indexes_3 = SRString::SplitFloats(elems[2], '/', 0, count + 1);

			face.x = indexes_1[0];
			face.y = indexes_2[0];
			face.z = indexes_3[0];

			if (count > 0) { // Texture coords
				uv.x = indexes_1[1];
				uv.y = indexes_2[1];
				uv.z = indexes_3[1];

			}
			// And
			if (count > 1) { // Normal coords
				norm_index.x = indexes_1[2];
				norm_index.y = indexes_2[2];
				norm_index.z = indexes_3[2];
			}

			switch (count) {
			case 1: // With texture
				m_temp_vertexes.push_back({ { m_pos_vertex[face.x - 1.f] }, m_pos_texture[uv.x - 1.f], {0,0,0}, {0,0,0} }); //z
				m_temp_vertexes.push_back({ { m_pos_vertex[face.y - 1.f] }, m_pos_texture[uv.y - 1.f], {0,0,0}, {0,0,0} }); //x
				m_temp_vertexes.push_back({ { m_pos_vertex[face.z - 1.f] }, m_pos_texture[uv.z - 1.f], {0,0,0}, {0,0,0} }); //y
				break; 
			case 2: // With normal
				//TODO: todo math tangents
				m_temp_vertexes.push_back({ { m_pos_vertex[face.x - 1.f] }, m_pos_texture[uv.x - 1.f], {0,0,0}, {0,0,0} }); //z
				m_temp_vertexes.push_back({ { m_pos_vertex[face.y - 1.f] }, m_pos_texture[uv.y - 1.f], {0,0,0}, {0,0,0} }); //x
				m_temp_vertexes.push_back({ { m_pos_vertex[face.z - 1.f] }, m_pos_texture[uv.z - 1.f], {0,0,0}, {0,0,0} }); //y
				break;
			default: break;
			}
		}
		int ObjLoader::ProcessLine(char* line) {
			m_line_number++;

			unsigned long len = SRString::FastStrLen(line);
			if (len > 1) {
				switch (line[0]) {
				case '#':
					break; // Comment
				case 'o':
					if (!m_current_object.empty()) {
						AddMesh();
						return 2; //TODO: temp solution
					}

					m_current_object = line[1];				
					break; // Object
				case 'g':
					if (!m_current_object.empty()) {
						AddMesh();
						return 2; //TODO: temp solution
					}

					m_current_object = line[1];
					break;
				case 'v':
					switch (line[1]) {
						case ' ': 
							m_pos_vertex.push_back(MakeVec3(line, ' ', 2 + (1 ? line[2] == ' ' : 0)));	break; // Vertex
						case 't': {
							//glm::vec2 vt = MakeVec2(line, ' ', 2 + (1 ? line[2] == ' ' : 0));
							//std::cout << vt.x << " " << vt.y << std::endl;
							m_pos_texture.push_back(MakeVec2(line, ' ', 2 + (1 ? line[2] == ' ' : 0)));	break; // Texture coord
						}
						case 'n': 
							m_pos_normal.push_back(MakeVec3(line, ' ', 3));								break; // Normal
						default: break;
					}
					break;
				case 'f': {
					char** elems = SRString::Split(line, ' ', 2, 3);

					ProcessFace(elems);

					delete[] elems[0];
					delete[] elems[1];
					delete[] elems[2];
					delete[] elems;
					break; // Face
				} 
				default: break;
				}
				return true;
			}
			else return false;
		}

		void ObjLoader::ProcessFile(const char* data) {
			long len = strlen(data), count = 0, last = 0;
			long line_number = 0;
			int pr_line_res = 0;
			for (int i = 0; i < len; i++) {
				count++;
				if (data[i] == '\n' || i + 1 == len) {
					line_number++;

					char* line = new char[count + 1]; strcpy(line, "");

					if (i + 1 == len) {
						strncat(line, data + last, count);
					}
					else
						strncat(line, data + last, count - 1);
					last = i + 1;

					pr_line_res = ProcessLine(line);
					if (pr_line_res == 2) { //TODO: temp solution
						break;
					}
						
					delete[] line;

					count = 0;
				}
			}
			AddMesh();
		}

		std::vector<Mesh*> ObjLoader::Load(std::string path) {
		ret: if (m_is_used) goto ret;
			m_is_used	= true;
			m_file_name = path;

			Debug::Log("ObjLoader::Load() : loading obj model \"" + m_file_name + "\"");

			m_temp_meshes.clear();

			Clear();

			LPCWSTR file = SRString::CharsToWchar(m_file_name.c_str());
			HANDLE hFile = CreateFile(file, GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

			if (!hFile) {
				Debug::Error("ObjLoader::Load() : file is not exists!\n\tPath : " + m_file_name);
				m_is_used = false;
				return m_temp_meshes;
			}

			DWORD dwFileSize = GetFileSize(hFile, NULL);
			HANDLE hFileMapping = CreateFileMappingA(hFile, NULL, PAGE_READONLY, 0, dwFileSize, NULL);
			if (!hFileMapping) {
				Debug::Error("ObjLoader::Load() : failed create file mapping!\n\tPath : " + m_file_name);
				m_is_used = false;
				return m_temp_meshes;
			}

			LPVOID lpFileMap = MapViewOfFile(hFileMapping,
				FILE_MAP_READ, 0, 0, 0);
			if (!lpFileMap) {
				Debug::Error("ObjLoader::Load() : map view of file!\n\tPath : " + m_file_name);
				m_is_used = false;
				return m_temp_meshes;
			}

			const char* data = (const char*)lpFileMap;

			{ // Process file
				ProcessFile(data);
				if (m_temp_meshes.size() == 0) {
					Debug::Error("ObjLoader::Load() : Failed load model!\n\tPath : " + m_file_name);
					m_is_used = false;
					Clear();
					return m_temp_meshes;
				}
			}

			UnmapViewOfFile(lpFileMap);
			CloseHandle(hFileMapping);

			delete[] file;
			CloseHandle(hFile);

			m_is_used = false;

			Clear();

			return m_temp_meshes;
		}
	}
}

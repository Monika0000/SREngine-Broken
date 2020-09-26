#include "pch.h"
#include "ObjLoader.h"
#include <Debug.h>
#include <SRString.h>
#include "ResourceManager.h"

namespace SpaRcle {
	using namespace Helper;

	namespace Graphics {
		bool ObjLoader::ProcessLine(char* line) {
			unsigned long len = SRString::FastStrLen(line);
			if (len > 1) {
				switch (line[0]) {
				case '#':
					break; // Comment
				case 'o':
					break; // Object
				case 'v':
					switch (line[1]) {
					case ' ': {
						float* vs = SRString::SplitFloats(line, ' ', 2 + (1 ? line[2] == ' ' : 0), 3);
						//std::cout << verts[0] << " " << verts[1] << " " << verts[2] << std::endl;
						delete[] vs;
						break; // Vertex
					}
					case 't': {
						float* vs = SRString::SplitFloats(line, ' ', 3, 2);

						delete[] vs;

						break; // Texture coord
					}
					case 'n': {
						float* vs = SRString::SplitFloats(line, ' ', 3, 3);

						delete[] vs;
						break; // Normal
					}
					default: break;
					}
					break;
				case 'f':

					break; // Face
				default: break;
				}
				return true;
			}
			else return false;
		}

		std::vector<Mesh*> ObjLoader::ProcessFile(const char* data) {
			long len = strlen(data), count = 0, last = 0;
			long line_number = 0;
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

					if (!ProcessLine(line)) {
						//Debug::Error("ObjLoader::ProcessFile() : Error line!\n\tLine number : "+
						//	std::to_string(line_number) + "\n\tPath : "+ObjLoader::m_file_name);
						Sleep(1);
					}
						
					delete[] line;

					count = 0;
				}
			}

			return std::vector<Mesh*>();
		}

		std::vector<Mesh*> ObjLoader::Load(std::string path) {
		ret: if (m_is_used) goto ret;
			m_is_used	= true;
			m_file_name = path;

			Debug::Log("ObjLoader::Load() : loading obj model \"" + m_file_name + "\"");

			std::vector<Mesh*> meshes = std::vector<Mesh*>();

			LPCWSTR file = SRString::CharsToWchar(m_file_name.c_str());
			HANDLE hFile = CreateFile(file, GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

			if (!hFile) {
				Debug::Error("ObjLoader::Load() : file is not exists!\n\tPath : " + m_file_name);
				m_is_used = false;
				return meshes;
			}

			DWORD dwFileSize = GetFileSize(hFile, NULL);
			HANDLE hFileMapping = CreateFileMappingA(hFile, NULL, PAGE_READONLY, 0, dwFileSize, NULL);
			if (!hFileMapping) {
				Debug::Error("ObjLoader::Load() : failed create file mapping!\n\tPath : " + m_file_name);
				m_is_used = false;
				return meshes;
			}

			LPVOID lpFileMap = MapViewOfFile(hFileMapping,
				FILE_MAP_READ, 0, 0, 0);
			if (!lpFileMap) {
				Debug::Error("ObjLoader::Load() : map view of file!\n\tPath : " + m_file_name);
				m_is_used = false;
				return meshes;
			}

			const char* data = (const char*)lpFileMap;

			{
				// Process file
				meshes = ProcessFile(data);
				if (meshes.size() == 0) {
					Debug::Error("ObjLoader::Load() : Failed load model!\n\tPath : " + m_file_name);
					m_is_used = false;
					return meshes;
				}
			}

			UnmapViewOfFile(lpFileMap);
			CloseHandle(hFileMapping);

			delete[] file;
			CloseHandle(hFile);

			m_is_used = false;

			return meshes;
		}
	}
}

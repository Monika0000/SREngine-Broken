#include "pch.h"
#include "ResourceManager.h"

#include "GameObject.h"
#include "Utils.h"
#include "SRFile.h"

using namespace SpaRcle;
using namespace Graphics;
using namespace Helper;

Mesh* SpaRcle::Graphics::ResourceManager::FindMesh(std::string file_name) {
	for (size_t t = 0; t < m_meshes.size(); t++)
		if (m_meshes[t]->m_file_name == file_name)
			return m_meshes[t];
	return nullptr;
}

bool SpaRcle::Graphics::ResourceManager::Init(std::string resource_path)
{
	if (m_isInitialize) {
		Debug::Error("ResourceManager::Init() : resource manager already initialized!");
		return false;
	}

	Debug::Info("Initializing resource manager...");

	std::vector<std::string> files = SRFile::GetAllFilesInDir(SRFile::GetPathToExe() + "\\..");
	for (auto a : files) {
		a = SRString::GetExtensionFromFilePath(a);

		if (a == "sln")
		{
			m_isDebug = true;
			break;
		}
	}

	if (resource_path == "") {
		if (m_isDebug)
			m_local_resource_path = "/../Resources";
		else
			m_local_resource_path = "/../../Resources";

		m_absolute_resource_path = SRString::MakePath(SRFile::GetPathToExe() + m_local_resource_path);
	}
	else {
		m_absolute_resource_path = resource_path;
	}

	m_isInitialize = true;

	Debug::System("ResourceManager::Init() : Set resource folder : " + m_absolute_resource_path);

	return true;
}
std::vector<Mesh*> SpaRcle::Graphics::ResourceManager::LoadObjModel(std::string name) {
	int counter = 0;
	std::string path = SRString::MakePath(ResourceManager::m_absolute_resource_path + "\\Models\\" + name + ".obj");

	std::vector<Mesh*> meshes = {};

	//auto find = m_meshes.find(path + " - " + std::to_string(0));
	auto find = FindMesh(path + " - " + std::to_string(0));
	//if (find != m_meshes.end()) {
	if (find != nullptr) {
		Debug::Log("ResourceManager::LoadObjModel() : loading \"" + path + "\" obj model from memory...");

		//Mesh* copy = find->second->Copy();
		Mesh* copy = find->Copy();
		m_meshes.push_back(copy);

		meshes.push_back(copy);

		ret:
			counter++;
			//find = m_meshes.find(path + " - " + std::to_string(counter));
			find = FindMesh(path + " - " + std::to_string(counter));
			//if (find != m_meshes.end()) {
			if (find != nullptr) {
				//copy = find->second->Copy();
				copy = find->Copy();
				meshes.push_back(copy);

				m_meshes.push_back(copy);
				goto ret;
			}

		return meshes;
	}
	else {
		meshes = ObjLoader::Load(path);

		for (auto a : meshes) {
			a->m_file_name = path + " - " + std::to_string(counter);
			m_meshes.push_back(a);
			//m_meshes.insert(std::make_pair(path + " - " + std::to_string(counter), a));
			counter++;
		}
		return meshes;
	}
}

SpaRcle::Graphics::Texture* SpaRcle::Graphics::ResourceManager::LoadTexture(std::string name, Texture::Type type, Texture::Filter filter) {
	name = ResourceManager::GetAbsoluteResourceFolder() + "\\Textures\\" + name;
	name = SRString::MakePath(name);
	
	auto find = m_textures.find(name);
	if (find != m_textures.end()) return find->second;

	Debug::Log("ResourceManager::LoadTexture() : loading \""+name+"\" texture...");

	std::string ext = SRString::BackReadToChar(name, '.');

	Image* image = nullptr;

	if (ext == "png")
		image = Image::LoadPNG(name.c_str());
	else if (ext == "tif" || ext == "tiff")
		image = Image::LoadTIFF(name.c_str());
	else if (ext == "jpg")
		image = Image::LoadJPG(name.c_str());
	else {
		Debug::Error("ResourceManager::LoadTexture() : Unknown image format!\n\tPath : " + std::string(name) + "\n\tExtension : " + ext);
		return nullptr;
	}

	if (!image || !image->m_data) {
		if (!image)
			Debug::Error("ResourceManager::LoadTexture() : Failed loading image!\n\tPath : " + std::string(name)
				+ "\n\tImage : " + std::to_string((size_t)image) + "\n\tExtension : " + ext);
		else
			Debug::Error("ResourceManager::LoadTexture() : Failed loading image!\n\tPath : " + std::string(name)
				+ "\n\tImage : " + std::to_string((size_t)image) + "\n\tData : "
				+ std::to_string((size_t)image->m_data) + "\n\tExtension : " + ext);
		return nullptr;
	}
	//else if (image->m_data[0] == '\0') {
	//	Debug::Error("ResourceManager::LoadTexture() : failed loading texture! image->m_data[0] == '0'!");
	//	return nullptr;
	//}

	image->m_name = name;

	Texture* texture = new Texture(image, type, filter);

	m_textures.insert(std::make_pair(name, texture));

	return texture;
}

GameObject* SpaRcle::Graphics::ResourceManager::LoadPrefab(std::string file_name, std::string gm_name) {
	if (gm_name == "Random name")
		gm_name = "New GameObject (" + std::to_string(m_gameObjects.size() + 1) + ")";

	file_name = ResourceManager::GetAbsoluteResourceFolder() + "\\Prefabs\\" + file_name + ".prefab";

	if (!SRFile::FileExists(file_name)) {
		Debug::Error("ResourceManager::LoadPrefab() : file \""+file_name+"\" not exists!");
		return nullptr;
	}
	else {
		Debug::Log("ResourceManager::LoadPrefab() : loading \"" + file_name + "\" prefab...");

		GameObject* gm = GameObject::Instance(gm_name);
		if (!gm) {
			Debug::Error("ResourceManager::LoadPrefab() : failed instance GameObject!\n\tFile : \"" + file_name + "\"");
			return nullptr; 
		}

		std::ifstream is(file_name);
		if (!is.is_open()) {
			Debug::Error("ResourceManager::LoadPrefab() : failed open \"" + file_name + "\" file!");
			return nullptr;
		}
		std::string line = "";

		std::vector<std::string> args = {};

		while (!is.eof()) {
			//std::cout << "\"" << line << "\"" << std::endl;

			if (line != "") {
				args = SRString::Split(line + " ", " ");
				std::cout << SRString::ArrayToLine(args, "|") << std::endl;
			}

			std::getline(is, line);
		}

		is.close();

		return gm;
	}
}

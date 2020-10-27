#include "pch.h"
#include "ResourceManager.h"
#include "Transform.h"

#include "GameObject.h"
#include "Utils.h"
#include "SRFile.h"
#include <stack>

using namespace SpaRcle;
using namespace Graphics;
using namespace Helper;

int SpaRcle::Graphics::ResourceManager::FindSelectedGameObject(GameObject* gm) {
	for (size_t t = 0; t < ResourceManager::m_selected_gameObjects.size(); t++)
		if (m_selected_gameObjects[t] == gm)
			return t;
	return -1;
}

std::vector<GameObject*> SpaRcle::Graphics::ResourceManager::GetAllSelectedGameObjects() {
	return ResourceManager::m_selected_gameObjects;
}

void SpaRcle::Graphics::ResourceManager::AddSelectedGameObject(GameObject*gm) {
	//ResourceManager::m_selected_gameObjects.insert(std::make_pair(gm, false));
	if (FindSelectedGameObject(gm) == -1)
		m_selected_gameObjects.push_back(gm);
}

void SpaRcle::Graphics::ResourceManager::RemoveSelectedGameObject(GameObject*gm) {
	//ResourceManager::m_selected_gameObjects.erase(gm);
	int id = FindSelectedGameObject(gm);
	if (id != -1)
		m_selected_gameObjects.erase(m_selected_gameObjects.begin() + id);
}

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

SpaRcle::Graphics::Texture* SpaRcle::Graphics::ResourceManager::LoadTexture(std::string name, Texture::Type type, Texture::Filter filter, bool isAbsPath) {
	if (!isAbsPath)
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
		GameObject* current_gm = nullptr;
		//std::vector<GameObject*> stack = {};
		std::stack<GameObject*> stack;

		std::ifstream is(file_name);
		if (!is.is_open()) {
			Debug::Error("ResourceManager::LoadPrefab() : failed open \"" + file_name + "\" file!");
			return nullptr;
		}
		std::string line = "";

		std::vector<std::string> args = {};

		//!=============================== Resources ===============================
		std::map<std::string, Material*>				materials	= {}; 
		std::map<std::string, std::vector<Mesh*>>		meshes		= {}; unsigned int count_uses = 0;
		//!=============================== Resources ===============================

		while (!is.eof()) {
			if (line != "") {
				args = SRString::Split(line + " ", " ");
				if (args.size() > 0) {
					if (args[0] == "Material") {
						materials.insert(std::make_pair(args[1], ResourceManager::LoadMaterial(args[3])));
					}
					else if (args[0] == "Meshes") {
						if (SRString::GetExtensionFromFilePath(args[3]) == "obj") {
							std::string file_name = SRString::Remove(args[3], '.');
							meshes.insert(std::make_pair(args[1], ResourceManager::LoadObjModel(file_name)));
						}
					}

					else if (args[0] == "Begin_GameObject:") {
						if (!current_gm) {
							current_gm = gm;
							if (gm_name == "None")
								current_gm->SetName(args[1]);
							stack.push(current_gm);
						}
						else {
							GameObject* load = GameObject::Instance(args[1]);
							current_gm->AddChild(load);
							current_gm = load;

							stack.push(current_gm);
						}
					}
					else if (args[0] == "End_GameObject") {
						stack.pop();
						if (stack.size() > 0)
							current_gm = stack.top();
					}

					else if (args[0] == "Component<Mesh>") {
						args = SRString::Split(args[2], ",");
						int id = std::stoi(args[1].c_str());
						
						Mesh* mesh = nullptr;
						//Mesh* mesh = meshes[args[0]][id];

						//Mesh* mesh = meshes[args[0]][id]->Copy();
						if (count_uses == 0) {
							mesh = meshes[args[0]][id];
							count_uses++;
						}
						else {
							mesh = meshes[args[0]][id]->Copy();
							count_uses++;
						}

						if (args[2] != "null")
							mesh->SetMaterial(materials[args[2]]);
						if (current_gm)
							current_gm->AddComponent(mesh);
					}

					else if (args[0] == "Scale:") {
						args = SRString::Split(args[1], ",");
						glm::vec3 val = { std::stod(args[0]), std::stod(args[1]), std::stod(args[2]) };
						if (current_gm)
							current_gm->GetTransform()->SetScale(val);
					}
					else if (args[0] == "Position:") {
						args = SRString::Split(args[1], ",");
						glm::vec3 val = { std::stod(args[0]), std::stod(args[1]), std::stod(args[2]) };
						if (current_gm)
							current_gm->GetTransform()->SetPosition(val);
					}
				}
			}
			std::getline(is, line);
		}

		//gm->AddComponents(meshes.begin()->second);

		is.close();

		return gm;
	}
}

Material* SpaRcle::Graphics::ResourceManager::LoadMaterial(std::string name) {
	Debug::Log("ResourceManager::LoadMaterial() : loading " + name + " material...");

	std::string path = ResourceManager::GetAbsoluteResourceFolder() + "\\Materials\\" + name + ".mat";
	bool transparent = false;
	std::vector<Texture*> texs = std::vector<Texture*>(4);

	if (SRFile::FileExists(path)) {
		std::ifstream file(path);
		std::string line = "";

		if (file.is_open()) {
			std::getline(file, line);

			while (!file.eof()) {
				if (line != "") {
					if (line == "#transparent")
						transparent = true;
					else if (SRString::Remove(line, ' ') == "#diffuse")
						texs[0] = ResourceManager::LoadTexture(SRString::Substring(line, ' '));
					else if (SRString::Remove(line, ' ') == "#normal")
						texs[1] = ResourceManager::LoadTexture(SRString::Substring(line, ' '));
					else if (SRString::Remove(line, ' ') == "#specular")
						texs[2] = ResourceManager::LoadTexture(SRString::Substring(line, ' '));
					else if (SRString::Remove(line, ' ') == "#glossiness")
						texs[3] = ResourceManager::LoadTexture(SRString::Substring(line, ' '));
				}

				std::getline(file, line);
			}

			file.close();
		}
		else {
			Debug::Error("ResourceManager::LoadMaterial() : failed loading \""+path+"\" material!\n\tReason : unknown error!");
			return nullptr;
		}
	}
	else {
		Debug::Error("ResourceManager::LoadMaterial() : failed loading material!\n\tReason : file \""+path+"\" is not exists!");
		return nullptr;
	}

	return ResourceManager::CreateMaterial(transparent, texs, name, false);
}

Skybox* SpaRcle::Graphics::ResourceManager::LoadSkybox(std::string name, std::string img_format) {
	Debug::Log("ResourceManager::LoadSkybox() : loading \"" + name + "\" skybox...");

	static const std::string files[6]{ "_right", "_left", "_top", "_bottom", "_front", "_back" };

	std::vector<Image*> sides = std::vector<Image*>(6);

	for (unsigned char c = 0; c < 6; c++) {
		Image* side = nullptr;
		if (img_format == ".jpg")
			side = Image::LoadJPG((ResourceManager::GetAbsoluteResourceFolder() + "\\Skyboxes\\" + name + "\\skybox" + files[c] + img_format).c_str());
		else if (img_format == ".png")
			side = Image::LoadPNG((ResourceManager::GetAbsoluteResourceFolder() + "\\Skyboxes\\" + name + "\\skybox" + files[c] + img_format).c_str());
		else {
			Debug::Error("ResourceManager::LoadSkybox() : failed loading \"" + name + "\" skybox!\nReason : unknown \"" +img_format+ "\" format!");
			return nullptr;
		}

		if (side)
			sides[c] = side;
		else {
			Debug::Error("ResourceManager::LoadSkybox() : failed loading \"" + name + "\" skybox! Image is nullptr!");
			return nullptr;
		}
	}

	return new Skybox(sides);
}

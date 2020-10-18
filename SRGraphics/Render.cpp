#include "pch.h"
#include "Render.h"
#include <Debug.h>
#include "ResourceManager.h"
#include <list>
#include "Window.h"

using namespace SpaRcle::Helper;

void SpaRcle::Graphics::Render::SortTransparentMeshes() {
	//struct DepthSorter
//{
//	bool operator() (Mesh* i, Mesh* j)
//	{
//		return (i->Dist() < j->Dist());
//	}
//} sort_object;

//std::sort(m_meshes.begin(), m_meshes.end(), DepthSorter(sort_object));
}



void SpaRcle::Graphics::Render::AddMesh(Mesh* mesh) {
	if (mesh->GetMaterial()->IsTransparent()) {
		this->m_count_transparent_meshes++;
		this->m_transparent_meshes.push_back(mesh);
	}
	else {
		this->m_meshes.push_back(mesh);
		this->m_count_meshes++;
	}
}
void SpaRcle::Graphics::Render::AddMeshes(std::vector<Mesh*>& meshes) {
	for (auto& a : meshes) {
		if (a->GetMaterial()->IsTransparent()) {
			this->m_count_transparent_meshes++;
			this->m_transparent_meshes.push_back(a);
		}
		else {
			this->m_meshes.push_back(a);
			this->m_count_meshes++;
		}
	}
}

SpaRcle::Graphics::Window* SpaRcle::Graphics::Render::GetWindow() {
	if (!m_window) {
		Debug::Error("Render::GetWindow() : window is nullptr!");
		return nullptr;
	}
	else
		return m_window;
}

bool SpaRcle::Graphics::Render::Create(Window* window) {
	if (m_isCreated) {
		Debug::Error("Render::Create() : render already created!");
		return false;
	} else Debug::Graph("Creating render...");

	this->m_window = window;

	this->m_canvas = new Canvas();

	this->m_isCreated = true;

	return true;
}

/*
	Call only from window thread
*/
bool SpaRcle::Graphics::Render::Init() {
	if (m_isInit) {
		Debug::Error("Render::Init() : render already initialize!");
		return false;
	} else Debug::Graph("Initializing render...");

	this->m_geometry_shader = new Shader("geometry_shader", this);
	if (!this->m_geometry_shader->Compile())
		return false;
	if (!this->m_geometry_shader->Linking())
		return false;
	ResourceManager::SetStandartShader(this->m_geometry_shader);

	this->m_isInit = true;

	return true;
}

/*
	Call only from window thread
*/
bool SpaRcle::Graphics::Render::Run() {
	if (m_isRunning) {
		Debug::Error("Render::Run() : render already running!");
		return false;
	}
	else Debug::Graph("Running render...");

	this->m_isRunning = true;

	return true;
}

/*
	Call only from window thread
*/
bool SpaRcle::Graphics::Render::Close() {
	if (!m_isRunning) {
		Debug::Error("Render::Close() : render is not running!");
		return false;
	} else Debug::Graph("Close render...");

	this->m_isRunning = false;

	this->m_geometry_shader->Destroy();

	return true;
}

void SpaRcle::Graphics::Render::DrawSkybox() {
	this->m_skybox->Draw();
}

void SpaRcle::Graphics::Render::PlayAnimators() {
	for (auto& animator : this->m_animators)
		animator->NextFrame();
}

void SpaRcle::Graphics::Render::DrawGeometry() {
	m_geometry_shader->Use();

	m_camera->UpdateShader(m_geometry_shader);

	if (m_has_meshes_to_remove) {
		m_removing_meshes_now = true;
		for (auto& mesh : m_meshes_to_remove) {
			bool find = false;

			for (size_t t = 0; t < m_meshes.size(); t++) {
				if (m_meshes[t] == mesh) {
					m_count_meshes--;
					m_meshes.erase(m_meshes.begin() + t);
					break;
				}
			}
			if (!find) for (size_t t = 0; t < m_transparent_meshes.size(); t++) {
				if (m_transparent_meshes[t] == mesh) {
					m_count_transparent_meshes--;
					m_transparent_meshes.erase(m_transparent_meshes.begin() + t);
					break;
				}
			}

			mesh->FreeOpenGLMemory();

			ResourceManager::Destroy(mesh);
		}

		m_meshes_to_remove.clear();

		m_has_meshes_to_remove = false;
		m_removing_meshes_now = false;
	}

	for (auto& mesh : m_meshes)
	{
		if (mesh)
			mesh->Draw();
	}

	for (auto& mesh : m_transparent_meshes)
	{
		if (mesh)
			mesh->Draw();
	}

	glUseProgram(0);
}

void SpaRcle::Graphics::Render::DrawGUI() {
	this->m_canvas->Draw();
}

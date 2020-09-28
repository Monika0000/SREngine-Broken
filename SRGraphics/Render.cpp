#include "pch.h"
#include "Render.h"
#include <Debug.h>
#include "ResourceManager.h"
#include <list>
#include "Window.h"

using namespace SpaRcle::Helper;

void SpaRcle::Graphics::Render::SortMeshes() {
	//struct DepthSorter
	//{
	//	bool operator() (Mesh* i, Mesh* j)
	//	{
	//		return (i->Dist() < j->Dist());
	//	}
	//} sort_object;

	//std::sort(m_meshes.begin(), m_meshes.end(), DepthSorter(sort_object));
}

void SpaRcle::Graphics::Render::AddMeshes(std::vector<Mesh*>& meshes) {
	this->m_count_meshes += meshes.size();
	for (auto& a : meshes)
		this->m_meshes.push_back(a);
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
	if (!this->m_geometry_shader->Linking())
		return false;
	if (!this->m_geometry_shader->Compile())
		return false; 
	ResourceManager::SetStandartGeometryShader(this->m_geometry_shader);

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

void SpaRcle::Graphics::Render::DrawGeometry() {
	//std::cout << m_meshes.size() << std::endl;
	for (auto& mesh : m_meshes)
	{
		mesh->Draw();
	}
}

void SpaRcle::Graphics::Render::DrawGUI() {
	this->m_canvas->Draw();
}

//#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS 1

#include "pch.h"
#include "Render.h"
#include <Debug.h>
#include "ResourceManager.h"
#include <list>
#include "Window.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "ColorBuffer.h"

using namespace SpaRcle::Helper;

void SpaRcle::Graphics::Render::SortTransparentMeshes() {
	std::vector<float> dist = {};
	glm::vec3 first, second;
	for (auto a : this->m_transparent_meshes) {
		first = a->m_position;
		second = m_camera->GetPosition();
		dist.push_back(sqrt(
			pow(second.x - first.x, 2) +
			pow(second.y - first.y, 2) +
			pow(second.z - first.z, 2)
		));
	}
	//for ()
}

void SpaRcle::Graphics::Render::AddMeshToCaclulate(Mesh* mesh) {
ret:if (m_calculate_meshes_now) goto ret;
	this->m_copy_meshes_calculate.push_back(mesh);
	this->m_has_copy_meshes_to_calc = true;
}

void SpaRcle::Graphics::Render::AddMesh(Mesh* mesh) {
ret:if (m_is_draw_now) goto ret;

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

	if (!m_skybox) {
		Debug::Error("Render::Create() : skybox is nullptr!");
		return false;
	}

	this->m_window = window;

	this->m_isCreated = true;

	return true;
}

/*
	Call only from window thread
*/
bool SpaRcle::Graphics::Render::Init() {
	if (!m_isCreated) {
		Debug::Error("Render::Init() : render is not created!");
		return false;
	}

	if (m_isInit) {
		Debug::Error("Render::Init() : render already initialize!");
		return false;
	} else Debug::Graph("Initializing render...");

	//this->m_skybox->SetCamera(m_camera);

	{
		this->m_geometry_shader = new Shader("geometry_shader", this);
		if (!this->m_geometry_shader->Compile())
			return false;
		if (!this->m_geometry_shader->Linking())
			return false;
	}

	{
		this->m_skybox_shader = new Shader("skybox_shader", this);
		if (!this->m_skybox_shader->Compile())
			return false;
		if (!this->m_skybox_shader->Linking())
			return false;
	}

	{
		this->m_selector_shader = new Shader("selector_shader", this);
		if (!this->m_selector_shader->Compile())
			return false;
		if (!this->m_selector_shader->Linking())
			return false;
	}

	{
		this->m_grid_shader = new Shader("grid_shader", this);
		if (!this->m_grid_shader->Compile())
			return false;
		if (!this->m_grid_shader->Linking())
			return false;
	}

	ResourceManager::SetStandartShader(this->m_geometry_shader);
	//ResourceManager::SetSkyboxShader(this->m_skybox_shader);

	this->m_isInit = true;

	return true;
}

/*
	Call only from window thread
*/
bool SpaRcle::Graphics::Render::Run() {
	if (!m_isInit) {
		Debug::Error("Render::Run() : render is not initialize!");
		return false;
	}

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
	this->m_selector_shader->Destroy();
	this->m_skybox_shader->Destroy();

	this->m_skybox->Destroy();

	return true;
}

SpaRcle::Graphics::Mesh* SpaRcle::Graphics::Render::GetAimedMesh() {
wait_complete_diff_task:
	if (m_find_aimed_mesh_stat == 0) {
		m_find_aimed_mesh_stat = 2;
	wait_find:
		if (m_find_aimed_mesh_stat == 2)
			goto wait_find;
		else if (m_find_aimed_mesh_stat == 1) {
			m_find_aimed_mesh_stat = 0;
			return m_current_aimed_mesh;
		}
		else if (m_find_aimed_mesh_stat == -1) {
			m_find_aimed_mesh_stat = 0;
			return nullptr;
		}
	}
	else
		goto wait_complete_diff_task;

	return nullptr;
}

void SpaRcle::Graphics::Render::PoolEvents() {
	if (m_has_copy_meshes_to_calc) {
		m_calculate_meshes_now = true;

		for (auto a : m_copy_meshes_calculate)
			a->Calculate();

		m_copy_meshes_calculate.clear();
		m_has_copy_meshes_to_calc = false;

		m_calculate_meshes_now = false;
	}

	if (m_has_meshes_to_remove) {
		m_removing_meshes_now = true;
		for (m_t = 0; m_t < m_meshes_to_remove.size(); m_t++) {
			bool find = false;

			for (size_t t = 0; t < m_meshes.size(); t++) {
				if (m_meshes[t] == m_meshes_to_remove[m_t]) {
					m_count_meshes--;
					m_meshes.erase(m_meshes.begin() + t);
					break;
				}
			}
			if (!find) for (size_t t = 0; t < m_transparent_meshes.size(); t++) {
				if (m_transparent_meshes[t] == m_meshes_to_remove[m_t]) {
					m_count_transparent_meshes--;
					m_transparent_meshes.erase(m_transparent_meshes.begin() + t);
					break;
				}
			}

			m_meshes_to_remove[m_t]->FreeOpenGLMemory();

			ResourceManager::Destroy(m_meshes_to_remove[m_t]);
		}

		m_meshes_to_remove.clear();

		m_has_meshes_to_remove = false;
		m_removing_meshes_now = false;
	}
}

void SpaRcle::Graphics::Render::FindAimedMesh() {
	if (m_find_aimed_mesh_stat == 2) {
		m_selector_shader->Use();

		m_camera->UpdateShader(m_selector_shader);

		for (m_t = 0; m_t < m_count_meshes; m_t++)
			m_meshes[m_t]->Draw();

		for (m_t = 0; m_t < m_count_transparent_meshes; m_t++) {
			m_transparent_meshes[m_t]->Draw();
		}

		glUseProgram(0);
	}
}

void SpaRcle::Graphics::Render::DrawGrid() {
	//m_grid_shader->Use();
	//m_camera->UpdateShader(m_grid_shader);
	//glDrawArrays(GL_TRIANGLES, 0, 0);
}

void SpaRcle::Graphics::Render::DrawSkybox() {
	if (m_skybox) {
		this->m_skybox_shader->Use();
		this->m_camera->UpdateShader(m_skybox_shader);
		this->m_skybox_shader->SetVec3("CamPos", m_camera->GetGLPosition());
		this->m_skybox->Draw();
		glUseProgram(0);
	}
}

void SpaRcle::Graphics::Render::PlayAnimators() {
	for (auto& animator : this->m_animators)
		animator->NextFrame();
}

void SpaRcle::Graphics::Render::DrawGeometry() {
	this->m_is_draw_now = true;

	m_geometry_shader->Use();

	m_camera->UpdateShader(m_geometry_shader);

	for (m_t = 0; m_t < m_count_meshes; m_t++)
		m_meshes[m_t]->Draw();

	SortTransparentMeshes();

	for (m_t = 0; m_t < m_count_transparent_meshes; m_t++) {
		m_transparent_meshes[m_t]->Draw();
	}

	glUseProgram(0);

	this->m_is_draw_now = false;
}

void SpaRcle::Graphics::Render::DrawGUI() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::GetStyle().WindowRounding = 0.0f;

	for (auto f : m_gui_elements)
		f.second();

	if (false)
	{
		ImGui::Begin("Window");

		std::vector<GameObject*> gms = ResourceManager::GetGameObjects();

		if (ImGui::TreeNode("Hierachy"))
		{
			ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 3);
			for (int i = 0; i < gms.size(); i++)
			{
				ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_NoTreePushOnOpen;// ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
				bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, gms[i]->GetName().c_str());
			}
			ImGui::TreePop();
			ImGui::PopStyleVar();
		}

		/*
		ImGui::ListBoxHeader("Hierarchy");
		for (auto item : ResourceManager::GetGameObjects())
		{
			if (ImGui::Selectable(item->GetName().c_str(), false))
			{
				// handle selection
			}
		}
		ImGui::ListBoxFooter();
		*/

		ImGui::End();
	}

	if (false)
	{
		ImGui::Begin("Window");

		ImGui::Text("Position");
		ImGui::NextColumn();
		ImGui::Text("Rotation");
		ImGui::NextColumn();
		ImGui::Text("Scale");

		ImGui::BeginTabBar("bar");	ImGui::EndTabBar();

		ImGui::End();
	}

	ImGui::Render();
	int display_w, display_h;
	glfwGetFramebufferSize(this->m_window->GetGLFWWindow(), &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	//ImGui::EndFrame();
	//ImGui::UpdatePlatformWindows();

	//ImGui::UpdatePlatformWindows();
	//ImGui::RenderPlatformWindowsDefault();
}

#include "pch.h"
#include "Camera.h"
#include "GameObject.h"
#include "Component.h"
#include "Shader.h"
#include "Window.h"

#include <Debug.h>

using namespace SpaRcle::Helper;

void SpaRcle::Graphics::Camera::OnMoved(glm::vec3 new_pos) {
    //std::cout << new_pos.x << " " << new_pos.y << " " << new_pos.z << std::endl;
    this->m_pos = new_pos;
    this->UpdateView();
}

void SpaRcle::Graphics::Camera::UpdateView() {
    this->m_viewMat = glm::lookAt(
        glm::vec3(0, 1, 3), 
        m_pos/*glm::vec3(0, 1, 2)*/, 
        glm::vec3(0, 1, 0)
    );
}

void SpaRcle::Graphics::Camera::UpdateShader(Shader* shader) noexcept {
    shader->SetMat4("viewMat", this->m_viewMat);
    shader->SetMat4("projMat", *this->m_projection);
}

bool SpaRcle::Graphics::Camera::Create(Window* window) {
    Debug::Graph("Camera::Create() : creating camera...");

    this->m_window = window;
    this->m_projection = &m_window->GetProjection();

    return true;
}

bool SpaRcle::Graphics::Camera::Init() {
    Debug::Graph("Camera::Init() : initializing camera...");

    return true;
}

bool SpaRcle::Graphics::Camera::Run() {
    Debug::Graph("Camera::Run() : running camera...");

    this->UpdateView();

    return true;
}

bool SpaRcle::Graphics::Camera::Close() {
    Debug::Graph("Camera::Close() : close camera...");

    return true;
}

#include "pch.h"
#include "Camera.h"
#include "GameObject.h"
#include "Component.h"
#include "Shader.h"
#include "Window.h"

#include <Debug.h>

using namespace SpaRcle::Helper;

void SpaRcle::Graphics::Camera::UpdateShader(Shader* shader) noexcept {
    shader->SetMat4("viewMat", glm::lookAt(glm::vec3(0, 1, 3), glm::vec3(0, 1, 2), glm::vec3(0, 1, 0)));
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

    return true;
}

bool SpaRcle::Graphics::Camera::Close() {
    Debug::Graph("Camera::Close() : close camera...");

    return true;
}

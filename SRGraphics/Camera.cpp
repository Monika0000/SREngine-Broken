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
    //this->m_pos = new_pos;
    //this->m_pos = { -new_pos.z, new_pos.y, new_pos.x };
    this->m_pos = { -new_pos.z, new_pos.y, new_pos.x };
    this->UpdateView();
}

void SpaRcle::Graphics::Camera::OnRotated(glm::vec3 new_rot) {
    this->m_yaw = new_rot.y * 3.14 / 45.f / 4.f;
    this->m_ptc = new_rot.z * 3.14 / 45.f / 4.f;

    //std::cout << new_rot.y << " " << new_rot.x << std::endl;

   // this->m_yaw = new_rot.y; // (new_rot.y * 3.14 / 45.f / 4.f);

    /*// �������������� ���������� �������� X-Z
    m_dxx = sin(new_rot.x); // dxx - ��������� ����� ������������ ��� X
    m_dxz = cos(new_rot.x); // dxz - ��������� ����� ������������ ��� Z
    m_dyy = sin(new_rot.y / 4.f) * 4.f; // dyy - ��������� ����� ������������ ��� Y

    float dx = new_rot.x - m_pos.x;
    float dy = new_rot.y - m_pos.y;
    float dz = new_rot.z - m_pos.z;

    m_pos.x += dz * m_dxx;
    m_pos.x += dx * m_dxx;
    m_pos.x += dz * m_dxz;
    m_pos.z -= dx * m_dxz;
    m_pos.y += dy / 10.f;
    */
    this->UpdateView();
}

void SpaRcle::Graphics::Camera::UpdateView() {
    /*
    GLfloat camX = sin(m_yaw);
    GLfloat camZ = cos(m_yaw);

    GLfloat camY = sin(m_ptc);

    //std::cout << camX << " " << camZ << std::endl;

    float posx =  m_pos.x;
    float posy =  m_pos.y;
    float posz = -m_pos.z;

    //glm::vec3 cameraVec = rotateZ(glm::vec3(dist, 0.0f, 0.0f), ang * timeSinceStart);

    //glm::vec3 eye = glm::vec3(0.0f, 1.0f, 3);

    //std::cout << m_pos.x << " " << m_pos.y << " " << m_pos.z << std::endl;

    const float eye_radius = 1.f / 100.f; // most correctly

    this->m_viewMat = glm::lookAt(
        { 
            glm::vec3(camX, camY, camZ) * eye_radius + glm::vec3(posx, posy, posz)
        },
        { posx, posy, posz },
        glm::vec3(0, 1, 0)
    );*/

    GLfloat camX = sin(m_yaw);
    GLfloat camY = tan(m_ptc);
    GLfloat camZ = cos(m_yaw);

    float posx = m_pos.x;
    float posy = m_pos.y;
    float posz = -m_pos.z;

    const float eye_radius = 1.f / 100.f; // most correctly

    this->m_viewMat = glm::lookAt(
        {
            glm::vec3(camX, camY, camZ) * eye_radius + glm::vec3(posx, posy, posz)
        },
        { posx, posy, posz },
        glm::vec3(0, 1, 0)
    );
}

void SpaRcle::Graphics::Camera::UpdateShader(Shader* shader) noexcept {
    if (!shader) return;

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

/*
    float dxx = sin(m_yaw);
    float dxz = cos(m_yaw);

    float posx = m_pos.x;
    float posy = m_pos.y;
    float posz = -m_pos.z;

    glm::vec3 eye = glm::vec3(0.0f, 1.0f, 3);

    std::cout << m_pos.x << " " << m_pos.y << " " << m_pos.z << std::endl;

    this->m_viewMat = glm::lookAt(
        {
            posx + 0,
            posy + 1,
            posz + 3
        },
        { posx + eye.x, posy + eye.y, posz - eye.x },
        glm::vec3(0, 1, 0)
    );
*/
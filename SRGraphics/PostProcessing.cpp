#include "PostProcessing.h"
#include "Shader.h"
#include "Render.h"
#include "Window.h"

#include <glm/glm.hpp>

using namespace SpaRcle::Helper;

SpaRcle::Graphics::PostProcessing::PostProcessing(Window* window) {
    this->m_window = window;
}

SpaRcle::Graphics::PostProcessing::~PostProcessing() {
    this->m_window = nullptr;
}

bool SpaRcle::Graphics::PostProcessing::Init(Shader* shader, GLuint* FBO, GLuint* screen_texture) {
    Debug::Graph("PostProcessing::Init() : initializing post processing...");

    this->m_shader          = shader;
    this->m_FBO             = FBO;
    this->m_ScreenTexture   = screen_texture;

    return true;
}

bool SpaRcle::Graphics::PostProcessing::Destoy() {
    Debug::Graph("PostProcessing::Destroy() : destroy post processing...");

    return true;
}

bool SpaRcle::Graphics::PostProcessing::Begin() {
    if (!m_isCaclucale) { Calculate(); return false; }

    glBindFramebuffer(GL_FRAMEBUFFER, *m_FBO);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    return true;
}

bool SpaRcle::Graphics::PostProcessing::Calculate() {
    if (m_isCaclucale) return false;

    Debug::Graph("PostProcessing::Calculate() : calculating post processing...");

    GLuint VBO;

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(QuadVertices), &QuadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glDeleteBuffers(1, &VBO);

    m_isCaclucale = true;

    return true;
}

bool SpaRcle::Graphics::PostProcessing::End() {
    if (!m_isCaclucale) return false;

    this->m_shader->Use();
    glBindVertexArray(m_VAO);
    glBindTexture(GL_TEXTURE_2D, *m_ScreenTexture);	// use the color attachment texture as the texture of the quad plane
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glUseProgram(0);

    return true;
}

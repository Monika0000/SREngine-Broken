#include "PostProcessing.h"
#include "Shader.h"
#include "Render.h"
#include "Window.h"

#include <glm/glm.hpp>

using namespace SpaRcle::Helper;

void SpaRcle::Graphics::PostProcessing::Resize(int w, int h){
    if (!m_pp_FBO) {
        glGenFramebuffers(1, &m_pp_FBO);
        Debug::Graph("PostProcessing::Resize() : frame buffer object has been created. Index : " + std::to_string(m_pp_FBO));
    }

    if (!m_screen_pp_texture) {
        glGenTextures(1, &m_screen_pp_texture);
        Debug::Graph("PostProcessing::Resize() : screen texture has been created. Index : " + std::to_string(m_screen_pp_texture));
    }

    glBindTexture(GL_TEXTURE_2D, m_screen_pp_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    //================================================================================================

    if (m_screen_pp_texture && m_pp_FBO) {
        glBindFramebuffer(GL_FRAMEBUFFER, m_pp_FBO);
        // присоедиение текстуры к объекту текущего кадрового буфера
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_screen_pp_texture, 0);
    }
    else {
        Debug::Error("PostProcessing::Resize() : screen texture or FBO is not created!");
        return;
    }

    if (!m_pp_RBO) {
        glGenRenderbuffers(1, &m_pp_RBO);
        Debug::Graph("PostProcessing::Resize() : render buffer object has been created. Index : " + std::to_string(m_pp_RBO));
    }

    glBindRenderbuffer(GL_RENDERBUFFER, m_pp_RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_pp_RBO);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        Debug::Error("PostProcessing::Resize() : Framebuffer is not complete!\n\tWidth = " +
            std::to_string(w) + "\n\tHeight = " + std::to_string(h) + "\n\tScreenTexture = " + std::to_string(m_screen_pp_texture) +
            "\n\tFBO = " + std::to_string(m_pp_FBO) + "\n\tRBO = " + std::to_string(m_pp_RBO));

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool SpaRcle::Graphics::PostProcessing::Use() {
    this->m_shader->Use();

    {
        m_shader->SetFloat("Gamma", this->m_gamma);
        m_shader->SetVec3("ColorCorrection", this->m_color_correction);
    }

    return true;
}

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

    this->m_isInit = true;

    return true;
}

bool SpaRcle::Graphics::PostProcessing::Destoy() {
    if (m_isDestroy || !m_isCaclucale) return false;

    Debug::Graph("PostProcessing::Destroy() : destroy post processing...");

    glDeleteBuffers(1, &this->m_VBO);
    glDeleteVertexArrays(1, &this->m_VAO);
    
    this->m_isDestroy = true;

    return true;
}

bool SpaRcle::Graphics::PostProcessing::Begin() {
    if (!m_isCaclucale) Calculate(); 

    glBindFramebuffer(GL_FRAMEBUFFER, *m_FBO);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    return true;
}

bool SpaRcle::Graphics::PostProcessing::Calculate() {
    if (m_isCaclucale) return false;

    Debug::Graph("PostProcessing::Calculate() : calculating post processing...");

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(QuadVertices), &QuadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    m_isCaclucale = true;

    return true;
}

bool SpaRcle::Graphics::PostProcessing::End() {
    if (!m_isCaclucale) return false;

    if (m_render_into_window) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        Use();

        glBindVertexArray(m_VAO);
        glBindTexture(GL_TEXTURE_2D, *m_ScreenTexture);	// use the color attachment texture as the texture of the quad plane

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);

        glUseProgram(0);
    }
    else {
        glBindFramebuffer(GL_FRAMEBUFFER, m_pp_FBO);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        {   
            Use();

            glBindVertexArray(m_VAO);
            glBindTexture(GL_TEXTURE_2D, *m_ScreenTexture);	// use the color attachment texture as the texture of the quad plane

            glDrawArrays(GL_TRIANGLES, 0, 6);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, 0);

            glUseProgram(0);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    return true;
}

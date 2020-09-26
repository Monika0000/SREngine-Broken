#include "pch.h"
#include "Shader.h"
#include <Debug.h>
#include "Render.h"
#include "Window.h"
#include "Camera.h"

using namespace SpaRcle::Helper;

SpaRcle::Graphics::Shader::Shader(std::string name, Render* render)
{
	this->m_render = render;
	this->m_name = name;
}

SpaRcle::Graphics::Camera* SpaRcle::Graphics::Shader::GetCamera() {
    return this->m_render->GetWindow()->GetCamera();
}

bool SpaRcle::Graphics::Shader::Linking() {
    if (m_isLinked) {
        Debug::Error("Shader::Linking() : shader \"" + m_name + "\" already linking!");
        return false;
    }
    else
        Debug::Shader("Linking shader \"" + m_name + "\"...");

    this->m_isLinked = true;

    return true;
}

bool SpaRcle::Graphics::Shader::Compile() {
    if (!m_isLinked) {
        Debug::Error("Shader::Compile() : before compiling a \"" + m_name + "\" shader, you need to link it!");
        return false;
    }

    if (m_isCompiled) {
        Debug::Error("Shader::Compile() : shader \"" + m_name + "\" already compile!");
        return false;
    }
    else
        Debug::Shader("Compile shader \"" + m_name + "\"...");

    this->m_isCompiled = true;

    return true;
}

bool SpaRcle::Graphics::Shader::Use() {
    if (!this->m_isCompiled) {
        Debug::Error("Shader::Compile() : before using a \"" + m_name + "\" shader, you need to compile it!");
        return false;
    }

    return true;
}

bool SpaRcle::Graphics::Shader::Destroy() {
    if (!m_isCompiled) {
        Debug::Error("Shader::Destroy() : shader \"" + m_name + "\" is not compile!");
        return false;
    }
    else if (m_isDestroy) {
        Debug::Error("Shader::Destroy() : shader \"" + m_name + "\" already destroyed!");
        return false;
    }
    else {
        Debug::Shader("Destroying shader \"" + m_name + "\"...");
        m_isDestroy = true;
    }

    return true;
}

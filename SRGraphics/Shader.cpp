#include "pch.h"
#include "Shader.h"
#include <Debug.h>
#include "Render.h"
#include "Window.h"
#include "Camera.h"

#include <Utils.h>

#include <iostream>
#include <fstream>

#include "ResourceManager.h"

#include <glm\gtc\type_ptr.hpp>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "SOIL.lib")

using namespace SpaRcle::Helper;

void SpaRcle::Graphics::Shader::SetMat4(const std::string& name, glm::mat4 mat) {
    glUniformMatrix4fv(glGetUniformLocation(m_ProgramID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void SpaRcle::Graphics::Shader::SetFloat(const std::string& key, float f) {
    glUniform1fv(glGetUniformLocation(this->m_ProgramID, key.c_str()), 1, &f);
}

SpaRcle::Graphics::Shader::Shader(std::string name, Render* render)
{
	this->m_render = render;
	this->m_name = name;
}

SpaRcle::Graphics::Camera* SpaRcle::Graphics::Shader::GetCamera() {
    return this->m_render->GetWindow()->GetCamera();
}

void SpaRcle::Graphics::Shader::SetVec3(const std::string key, glm::vec3 vec) {
    glUniform3fv(glGetUniformLocation(this->m_ProgramID, key.c_str()), 1, &vec[0]); 
}


bool SpaRcle::Graphics::Shader::Compile() {
    if (m_isCompiled) {
        Debug::Error("Shader::Compile() : shader \"" + m_name + "\" already compile!");
        return false;
    }
    else
        Debug::Shader("Compile shader \"" + m_name + "\"...");

    std::string path = ResourceManager::GetAbsoluteResourceFolder() + "\\Shaders\\";
    std::string vertex_path = path + m_name + "_vertex.glsl";
    std::string fragment_path = path + m_name + "_fragment.glsl";;
    
    if (!SRFile::FileExists(vertex_path)) {
        Debug::Error("Shader::Compile() : failed compile shader!\n\tReason : file \"" + vertex_path + "\" is not exists!");
        return false;
    } else if (!SRFile::FileExists(fragment_path)) {
        Debug::Error("Shader::Compile() : failed compile shader!\n\tReason : file \"" + vertex_path + "\" is not exists!");
        return false;
    }

    //! создаем шейдеры
    m_VertexShaderID   =   glCreateShader(GL_VERTEX_SHADER);
    m_FragmentShaderID =   glCreateShader(GL_FRAGMENT_SHADER);

    //! читаем вершинный шейдер из файла
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_path, std::ios::in);
    if (VertexShaderStream.is_open()) {
        std::string Line = "";
        while (getline(VertexShaderStream, Line))
            VertexShaderCode += "\n" + Line;
        VertexShaderStream.close();
    }

    //! читаем фрагментный шейдер из файла
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_path, std::ios::in);
    if (FragmentShaderStream.is_open()) {
        std::string Line = "";
        while (getline(FragmentShaderStream, Line))
            FragmentShaderCode += "\n" + Line;
        FragmentShaderStream.close();
    }

    std::string error;

    {
        // Компилируем вершинный шейдер
        Debug::Shader("Compiling vertex shader : " + vertex_path);
        char const* VertexSourcePointer = VertexShaderCode.c_str();
        glShaderSource(m_VertexShaderID, 1, &VertexSourcePointer, NULL);
        glCompileShader(m_VertexShaderID);

        // Устанавливаем параметры
        glGetShaderiv(m_VertexShaderID, GL_COMPILE_STATUS, &m_Result);
        glGetShaderiv(m_VertexShaderID, GL_INFO_LOG_LENGTH, &m_InfoLogLength);

        //?===========================================[ ERRORS ]==============================================
        if (m_InfoLogLength != 0) {
            std::vector<char> VertexShaderErrorMessage(m_InfoLogLength);
            glGetShaderInfoLog(m_VertexShaderID, m_InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
            if (VertexShaderErrorMessage.size() > 10)
                Debug::Error("Shader::Compile() : Failed compiling vertex shader!\n\tReason : " + std::string(VertexShaderErrorMessage.data()));
        }
        //?===================================================================================================
    }

    {
        // Компилируем фрагментный шейдер
        Debug::Shader("Compiling fragment shader : " + fragment_path);
        char const* FragmentSourcePointer = FragmentShaderCode.c_str();
        glShaderSource(m_FragmentShaderID, 1, &FragmentSourcePointer, NULL);
        glCompileShader(m_FragmentShaderID);

        // Устанавливаем параметры
        glGetShaderiv(m_FragmentShaderID, GL_COMPILE_STATUS, &m_Result);
        glGetShaderiv(m_FragmentShaderID, GL_INFO_LOG_LENGTH, &m_InfoLogLength);

        //?===========================================[ ERRORS ]==============================================
        if (m_InfoLogLength != 0) {
            std::vector<char> FragmentShaderErrorMessage(m_InfoLogLength);
            glGetShaderInfoLog(m_FragmentShaderID, m_InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
            if (FragmentShaderErrorMessage.size() > 10)
                Debug::Error("Shader::Compile() : Failed compiling fragment shader!\n\tReason : " + std::string(FragmentShaderErrorMessage.data()));
        }
         //?===================================================================================================
    }

    this->m_isCompiled = true;

    return true;
}

bool SpaRcle::Graphics::Shader::Linking() {
    if (!m_isCompiled) {
        Debug::Error("Shader::Linking() : before linking a \"" + m_name + "\" shader, you need to compile it!");
        return false;
    }

    if (m_isLinked) {
        Debug::Error("Shader::Linking() : shader \"" + m_name + "\" already linking!");
        return false;
    }
    else
        Debug::Shader("Linking shader \"" + m_name + "\"...");

    m_ProgramID = glCreateProgram();
    glAttachShader(m_ProgramID, m_VertexShaderID);
    glAttachShader(m_ProgramID, m_FragmentShaderID);
    glLinkProgram(m_ProgramID);

    // Устанавливаем параметры
    glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &m_Result);
    glGetProgramiv(m_ProgramID, GL_INFO_LOG_LENGTH, &m_InfoLogLength);

    //?===========================================[ ERRORS ]==============================================
    if (m_InfoLogLength != 0) {
        std::vector<char> ProgramErrorMessage(m_InfoLogLength);
        glGetProgramInfoLog(m_ProgramID, m_InfoLogLength, NULL, &ProgramErrorMessage[0]);
        std::string error = SRString::FromCharVector(ProgramErrorMessage);
        if (error.size() != 0) {
            size_t index = error.find("error");
            if (index == SRMath::size_t_max) {
                Debug::Warn("Shader::Linking() : Warning linking program! Reason : " + error);
                m_isLinked = true;
            }
            else Debug::Error("Shader::Linking() : Failed linking program! Reason : " + error);
        }
        else m_isLinked = true;
    }
    else m_isLinked = true;
    //?===================================================================================================

    glDeleteShader(m_VertexShaderID);
    glDeleteShader(m_FragmentShaderID);

    return true;
}

bool SpaRcle::Graphics::Shader::Use() {
    if (!this->m_isCompiled) {
        Debug::Error("Shader::Compile() : before using a \"" + m_name + "\" shader, you need to compile it!");
        return false;
    }

    glUseProgram(m_ProgramID);

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

    glDeleteProgram(m_ProgramID);

    return true;
}

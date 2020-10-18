#include "pch.h"
#include "Mesh.h"
#include "Camera.h"
#include <Debug.h>

#include <glm/vec3.hpp>
#include <glm/matrix.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "ResourceManager.h"

SpaRcle::Graphics::Mesh::Mesh(Shader* geometry_shader, Material* material, std::string name) {
    this->m_geometry_shader = geometry_shader;
    this->m_name            = name;

    if (m_material) {
        this->m_material = material;
    } else {
        this->m_material = ResourceManager::GetDefaultMaterial();
    }
    this->m_camera = m_geometry_shader->GetCamera();

    ReCalcModel();
}

/*
    call only opengl context
*/
bool SpaRcle::Graphics::Mesh::FreeOpenGLMemory() {
    if (m_is_calculated) {
        Debug::Log("Mesh::FreeOpenGLMemory() : free \""+m_name+"\" mesh OpenGL context memory...");

        auto find = VAO_Register_Buffer.find(m_VAO);
        if (find == VAO_Register_Buffer.end()) {
            Debug::Error("Mesh::Calculate() : This mesh \"" + m_name + "\" is not exists or not register, something went wrong...");
            return false;
        }
        else {
            if (find->second == 0) {
                Debug::Error("Mesh::Calculate() : The number of this mesh \"" + m_name + "\" is equal to zero, apparently, something is broken...");
                return false;
            }
            else if (find->second == 1) {
                glDeleteVertexArrays(1, &this->m_VAO);
                VAO_Register_Buffer.erase(find->first);
            }
            else
                find->second--;
        }
        //`glDeleteBuffers(1, &this->m_VBO);
        return true;
    }
    else
        return false;
}

void SpaRcle::Graphics::Mesh::OnMoved(glm::vec3 new_val) {
    this->m_position = new_val;
    ReCalcModel();
}

void SpaRcle::Graphics::Mesh::OnRotated(glm::vec3 new_val) {
    this->m_rotation = new_val;
    ReCalcModel();
}

void SpaRcle::Graphics::Mesh::OnScaled(glm::vec3 new_val) {
    this->m_scale = new_val;
    ReCalcModel();
}

void SpaRcle::Graphics::Mesh::ReCalcModel() {
    glm::mat4 modelMat = glm::mat4(1.0f);

    const float PI = 3.14;

    //modelMat = glm::eulerAngleZYZ(
    //    0 / 180.f * PI,
    //    90 / 180.f * PI,
    //    0 / 180.f * PI
    //);
    modelMat = glm::translate(modelMat, {
        -m_position.z, m_position.y, -m_position.x
        }); // { 0, -8, -25 }

    modelMat = glm::rotate(modelMat, glm::radians(m_rotation.x), glm::vec3(1, 0, 0));
    modelMat = glm::rotate(modelMat, glm::radians(m_rotation.y), glm::vec3(0, 1, 0));
    modelMat = glm::rotate(modelMat, glm::radians(m_rotation.z), glm::vec3(0, 0, 1));

    
    modelMat = glm::scale(modelMat, m_scale); //glm::vec3(0.1, 0.1, 0.1) * 10.f

    this->m_modelMat = modelMat;
}

SpaRcle::Graphics::Mesh* SpaRcle::Graphics::Mesh::Copy(bool copy_transform) {
    if (m_is_destroyed) {
        Debug::Error("Mesh::Copy() : mesh \""+m_name+"\" is destroyed!");
        return nullptr;
    }
ret: if (!m_is_calculated) goto ret;

    Debug::Log("Mesh::Copy() : copying \""+m_name+"\" mesh...");

    Mesh* mesh = new Mesh(this->m_geometry_shader, m_material, this->m_name);

    mesh->m_file_name           = m_file_name;

    mesh->m_camera              = m_camera;
    mesh->m_count_vertices      = m_count_vertices;
    mesh->m_vertexes            = m_vertexes;

    mesh->m_is_calculated       = true;
    auto find = VAO_Register_Buffer.find(m_VAO);
    if (find == VAO_Register_Buffer.end()) {
        delete mesh;
        Debug::Error("Mesh::Copy() : failed copy \""+m_name+"\"mesh! VAO is not found! Apparently, something is broken...");
        return nullptr;
    } else {
        mesh->m_VAO = m_VAO;
        find->second++;
    }

    mesh->m_position            = m_position;
    mesh->m_rotation            = m_rotation;
    mesh->m_scale               = m_scale;

    return mesh;
}

void SpaRcle::Graphics::Mesh::SetVertexArray(std::vector<Vertex>& vertexes) noexcept {
    this->m_is_calculated = false;
    this->m_count_vertices = vertexes.size();
    this->m_vertexes = vertexes;
}

/*
    call only resource manager
*/
bool SpaRcle::Graphics::Mesh::Destroy() noexcept {
    if (this->m_is_destroyed) return false;
    else this->m_is_destroyed = true;

   // Debug::Log("Mesh::Destroy() : destroytig mesh...");

    return true;
}

bool SpaRcle::Graphics::Mesh::Draw() {
    if (m_is_destroyed) return false;

    if (!m_is_calculated) Calculate();

    //static float  f = 0;
   // f -= 0.0001;
   // glm::mat4 modelMat = glm::mat4(1.0f);
    //modelMat = glm::translate(modelMat, { 0, -8, -25 });
    //modelMat = glm::translate(modelMat, { sin(f), sin(f), sin(f) });
  //  modelMat = glm::scale(modelMat, glm::vec3(0.1, 0.1, 0.1) * 10.f);
    //modelMat = glm::rotate(modelMat, f, glm::vec3(0, 90, 0));

    m_geometry_shader->SetMat4("modelMat", m_modelMat);

    if (m_material) {
        if (!this->m_material->Use()) {
            /*
                Вызывается только в том случае, если в ресурс менеджере был вызван метод уничтожения,
                и данный ресурс требует уничтожения ресурсо в OpenGL костексте
              */
            Debug::Log("Mesh::Draw() : free OpenGL resources material from \"" + m_name + "\" mesh.");

            delete m_material;
            m_material = nullptr;
            m_material = ResourceManager::GetDefaultMaterial();
        }
    } else
        m_material = ResourceManager::GetDefaultMaterial();

    /* draw geometry... */
    glBindVertexArray(this->m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, this->m_count_vertices); //Начиная с вершины 0 и рисуем count_vertices штуки. Всего => count_vertices/3 треугольника
    glBindTexture(GL_TEXTURE0, 0);

    return true;
}

bool SpaRcle::Graphics::Mesh::Calculate() {
    if (m_is_calculated) return false;

    Helper::Debug::Log("Mesh::Calculate() : Binding \""+m_name+"\" mesh... Has " + std::to_string(m_count_vertices) + " vertices.");

    /* Generating VAO and VBO */

    GLuint VBO = 0;

    glGenVertexArrays(1, &this->m_VAO);
    glGenBuffers(1, &VBO);

    auto find = VAO_Register_Buffer.find(m_VAO);
    if (find == VAO_Register_Buffer.end())
        VAO_Register_Buffer.insert(std::make_pair(m_VAO, 1));
    else
        Debug::Error("Mesh::Calculate() : This mesh \""+m_name+"\" already exists, something went wrong...");

    /* binding vertex array... */

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //? Binding vertex array
    glBufferData(
        GL_ARRAY_BUFFER, 
        this->m_count_vertices * sizeof(Vertex), 
        &m_vertexes[0], 
        GL_STATIC_DRAW
    );

    //? Binding attrib vertex coordinates
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,
        3, // glm::vec3 - has 3 floats
        GL_FLOAT, GL_FALSE, 
        sizeof(Vertex), (void*)0);

    //? Binding attrib texture coordinates
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,
        2, // glm::vec2 - has 2 floats
        GL_FLOAT, GL_FALSE, sizeof(Vertex),
        (void*)offsetof(Vertex, texCoords) // Сдвиг байт до соответствующего атрибута
    );

    //? Binding attrib normal coordinates
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3,
        3, // glm::vec3 - has 3 floats
        GL_FLOAT, GL_FALSE,
        sizeof(Vertex), 
        (void*)offsetof(Vertex, normal) // Сдвиг байт до соответствующего атрибута
    );

    //? Binding attrib tangent coordinates
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4,
        3, // glm::vec3 - has 3 floats
        GL_FLOAT, GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, tangent) // Сдвиг байт до соответствующего атрибута
    );

    glBindVertexArray(0);
    glDeleteBuffers(1, &VBO);

    m_is_calculated = true;
    return true;
}

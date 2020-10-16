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

void SpaRcle::Graphics::Mesh::SetVertexArray(std::vector<Vertex>& vertexes) noexcept {
    this->m_is_calculated = false;
    this->m_count_vertices = vertexes.size();
    this->m_vertexes = vertexes;
}

bool SpaRcle::Graphics::Mesh::Destroy() noexcept {
    if (this->m_is_destroyed) return false;
    else this->m_is_destroyed = true;

    return true;
}

bool SpaRcle::Graphics::Mesh::Draw() {
    if ((unsigned long long)this > 0xFFFFFFFFFFFFF000)
    {
        return false;
    }

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

    this->m_material->Use();

    //glBindTexture(GL_TEXTURE_2D, 2);

    /* draw geometry... */
    glBindVertexArray(this->m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, this->m_count_vertices); //������� � ������� 0 � ������ count_vertices �����. ����� => count_vertices/3 ������������

    glBindTexture(GL_TEXTURE0, 0);

    return true;
}

bool SpaRcle::Graphics::Mesh::Calculate() {
    if (m_is_calculated) return false;

    Helper::Debug::Log("Mesh::Calculate() : Binding \""+m_name+"\" mesh... Has " + std::to_string(m_count_vertices) + " vertexes.");

    /* Generating VAO and VBO */

    glGenVertexArrays(1, &this->m_VAO);
    glGenBuffers(1, &this->m_VBO);

    /* binding vertex array... */

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

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
        (void*)offsetof(Vertex, texCoords) // ����� ���� �� ���������������� ��������
    );

    //? Binding attrib normal coordinates
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3,
        3, // glm::vec3 - has 3 floats
        GL_FLOAT, GL_FALSE,
        sizeof(Vertex), 
        (void*)offsetof(Vertex, normal) // ����� ���� �� ���������������� ��������
    );

    //? Binding attrib tangent coordinates
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4,
        3, // glm::vec3 - has 3 floats
        GL_FLOAT, GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, tangent) // ����� ���� �� ���������������� ��������
    );

    glBindVertexArray(0);

    m_is_calculated = true;
    return true;
}

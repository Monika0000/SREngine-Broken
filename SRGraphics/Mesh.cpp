#include "pch.h"
#include "Mesh.h"
#include "Camera.h"
#include <Debug.h>

SpaRcle::Graphics::Mesh::Mesh(Shader* geometry_shader, Material* material) {
    this->m_geometry_shader = geometry_shader;
    this->m_material = material;
    this->m_camera = m_geometry_shader->GetCamera();
}

void SpaRcle::Graphics::Mesh::SetVertexArray(std::vector<Vertex>& vertexes) noexcept {
    this->m_is_calculated = false;
    this->m_count_vertex = vertexes.size();
    this->m_vertexes = vertexes;
}

bool SpaRcle::Graphics::Mesh::Destroy() noexcept {
    if (this->m_is_destroyed) return false;
    else this->m_is_destroyed = true;

    return true;
}

bool SpaRcle::Graphics::Mesh::Draw() {
    if (m_is_destroyed) return false;

    if (!m_is_calculated) Calculate();

    /*
    
        draw geometry...
    
    */

    return true;
}

bool SpaRcle::Graphics::Mesh::Calculate() {
    if (m_is_calculated) return false;

    Helper::Debug::Log("Mesh::Calculate() : Binding new mesh... Has " + std::to_string(m_count_vertex) + " vertexes.");

    /* Generating VAO and VBO */

    glGenVertexArrays(1, &this->m_VAO);
    glGenBuffers(1, &this->m_VBO);

    /* binding vertex array... */

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    // Binding vertex array
    glBufferData(
        GL_ARRAY_BUFFER, 
        this->m_count_vertex * sizeof(Vertex), 
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

    m_is_calculated = true;
    return true;
}

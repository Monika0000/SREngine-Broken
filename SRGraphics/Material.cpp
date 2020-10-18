#include "pch.h"
#include "Material.h"
#include "Shader.h"

bool SpaRcle::Graphics::Material::Use() {
	if (m_is_destroy)
		return false;

	if (m_diffuse) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_diffuse->GetID());
		glUniform1i(glGetUniformLocation(m_fragment_shader->m_ProgramID, "DiffuseMap"), 0); // This is GL_TEXTURE0
	}
	else {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glUniform1i(glGetUniformLocation(m_fragment_shader->m_ProgramID, "DiffuseMap"), 0); // This is GL_TEXTURE0
	}

	return true;
}

SpaRcle::Graphics::Material::Material(bool transparent, Shader* shader) : Material(transparent, shader, {}) {}
SpaRcle::Graphics::Material::Material(bool transparent, Shader* shader, std::vector<Texture*> textures) : m_transparent(transparent) {
	this->m_fragment_shader = shader;
	if (textures.size() > 0) {
		this->m_diffuse = textures[0];
	}
}

bool SpaRcle::Graphics::Material::Destroy() {
	if (m_is_destroy) return false;

	m_is_destroy = true;

	this->m_fragment_shader = nullptr;
	this->m_diffuse = nullptr;
	this->m_normal = nullptr;
	this->m_specular = nullptr;
	this->m_glossiness = nullptr;

	return true;
}

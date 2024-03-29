#include "pch.h"
#include "Material.h"
#include "Shader.h"

#include "ResourceManager.h"

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

SpaRcle::Graphics::Material::Material(bool transparent, Shader* shader, const std::string m_dictionary_name, std::string name, bool isDefault)
	: Material(transparent, shader, m_dictionary_name, {}, name, isDefault) {}
SpaRcle::Graphics::Material::Material(bool transparent, Shader* shader, const std::string m_dictionary_name, std::vector<Texture*> textures, std::string name, bool isDefault)
	: m_transparent(transparent), m_name(name), m_isDefault(isDefault), m_dictionary_name(m_dictionary_name)
{
	this->m_fragment_shader = shader;
	short size = textures.size();
	if (size > 0)
		this->m_diffuse = textures[0];
	if (size > 1)
		this->m_normal = textures[1];
	if (size > 2)
		this->m_specular = textures[2];
	if (size > 3)
		this->m_glossiness = textures[3];
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

SpaRcle::Graphics::Material* SpaRcle::Graphics::Material::Copy() {
	return ResourceManager::CreateMaterial(m_transparent, {m_diffuse, m_normal, m_specular, m_glossiness}, m_name, m_isDefault);
}

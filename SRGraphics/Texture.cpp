#include "pch.h"
#include "Texture.h"
#include <Debug.h>

SpaRcle::Graphics::Texture::Texture(Image* image, Type type, Filter filter) {
	this->m_image	= image;
	this->m_filter	= filter;
	this->m_type	= type;
}

bool SpaRcle::Graphics::Texture::Destroy() {
	if (this->m_is_destroyed)
		return false;

	{



	}

	this->m_is_destroyed = true;

	return true;
}

bool SpaRcle::Graphics::Texture::Calculate() {
	if (m_is_calculated) return false;

	glActiveTexture(GL_TEXTURE0);

	// Индекс текстуры
	glGenTextures(1, &m_id);

	Helper::Debug::Log("Texture::Calculate() : calculating \"" + m_image->m_name + "\" ("+std::to_string(m_id)+") texture...");

	glBindTexture(GL_TEXTURE_2D, m_id);
	GLuint texParam = GL_CLAMP_TO_BORDER; // GL_REPEAT, GL_CLAMP_TO_BORDER
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texParam);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texParam);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLuint)m_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLuint)m_filter);
	
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, m_image->m_width, m_image->m_height, m_image->m_format,
		GL_UNSIGNED_BYTE, (unsigned int*)m_image->m_data);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glBindTexture(GL_TEXTURE_2D, 0);

	delete m_image;

	m_is_calculated = true;
	return true;
}

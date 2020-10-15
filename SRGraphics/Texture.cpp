#include "pch.h"
#include "Texture.h"

bool SpaRcle::Graphics::Texture::Calculate() {
	if (m_is_calculated) return false;

	//m_id = SOIL_create_OGL_texture(image->data, image->width, image->height, image->channels, 0,
	//	SOIL_FLAG_INVERT_Y | SOIL_FLAG_MIPMAPS);
	//glBindTexture(GL_TEXTURE_2D, textureID);


	GLuint texParam = GL_CLAMP_TO_BORDER; // GL_REPEAT, GL_CLAMP_TO_BORDER
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texParam);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texParam);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLuint)filter);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLuint)filter);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	return false;
}

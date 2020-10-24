#include "pch.h"
#include "Skybox.h"

#include <Debug.h>

bool SpaRcle::Graphics::Skybox::Calculate() {
	if (m_is_calculate || m_is_destroy) return false;

	Helper::Debug::Log("Skybox::Calculate() : calculating skybox...");

	glGenTextures(1, &m_Cubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_Cubemap);

	for (unsigned char c = 0; c < 6; c++) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + c,
			0, GL_RGB, m_sides[c]->m_width, m_sides[c]->m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_sides[c]->m_data
		);
		delete m_sides[c];
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	//?/////////////////////////////////////////////////////////////////////////////////

	GLuint VBO = 0;

	glGenVertexArrays(1, &this->m_VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);

	m_is_calculate = true;

	return true;
}

//void SpaRcle::Graphics::Skybox::SetCamera(Camera* cam) {
//	this->m_camera = cam;
//}

void SpaRcle::Graphics::Skybox::Draw() {
	if (m_is_destroy) return;
	if (!m_is_calculate) Calculate();

	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	// ... задание видовой и проекционной матриц
	glBindVertexArray(m_VAO);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->m_Cubemap);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthFunc(GL_LESS); // set depth function back to default
}

bool SpaRcle::Graphics::Skybox::Destroy() {
	if (!m_is_calculate || m_is_destroy) return false;

	Helper::Debug::Graph("Skybox::Destroy() : destroying skybox...");

	glDeleteVertexArrays(1, &this->m_VAO);
	glDeleteTextures(1, &m_Cubemap);

	this->m_is_destroy = true;

	return true;
}

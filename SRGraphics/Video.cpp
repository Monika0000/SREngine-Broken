#include "pch.h"
#include "Video.h"
extern "C" {
#include "libavcodec/avcodec.h"
#include "libavutil/imgutils.h"
#include "libswscale/swscale.h"
#include "libavformat/avformat.h"
#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"
}
#include <Windows.h>
#include <filesystem>
#include "Texture.h"
#include "Shader.h"
#include "ResourceManager.h"

SpaRcle::Graphics::Video::Video(std::string file_name, PlayMode play_mode) : Material(false, ResourceManager::GetStandartShader()) {
	this->m_playMode = play_mode;
	this->m_file_name = file_name;

	if (!Load())
		Debug::Error("Video::Constructor() : failed load video \"" + m_file_name + "\", this data structure delete needed!");
}

void SpaRcle::Graphics::Video::Use() {
	if (!m_is_calculate) Calculate();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_frame_id);
	glUniform1i(glGetUniformLocation(m_fragment_shader->m_ProgramID, "DiffuseMap"), 0);
}


bool SpaRcle::Graphics::Video::Calculate() {
	if (m_is_calculate) return false;
	/*
	glActiveTexture(GL_TEXTURE0);
	// Индекс текстуры
	glGenTextures(1, &m_frame_id);

	glBindTexture(GL_TEXTURE_2D, m_frame_id);
	GLuint texParam = GL_CLAMP_TO_BORDER; // GL_REPEAT, GL_CLAMP_TO_BORDER
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texParam);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texParam);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLuint)GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLuint)GL_NEAREST);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, m_width, m_height, GL_RGB,
		GL_UNSIGNED_BYTE, m_temp_data);

	glBindTexture(GL_TEXTURE_2D, 0);
	*/
	this->m_is_calculate = true;
	return true;
}

bool SpaRcle::Graphics::Video::Load() {
	std::string res = ResourceManager::GetResourceFolder();
	std::string folder = SRString::GetFileNameFromPath(m_file_name);

	CreateDirectory(SRString::CharsToWchar((res + "\\Cache\\" + folder).c_str()), NULL);

	system((res + "\\Utilites\\ffmpeg\\ffmpeg.exe -loglevel quiet -hide_banner -i "+this->m_file_name+" "+res+"\\Cache\\"+folder+"\\frame%d.png").c_str());

	return true;
}
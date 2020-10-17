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
#include "Texture.h"
#include "Shader.h"
#include "ResourceManager.h"

#include <SRFile.h>

#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;

#include <Input.h>

SpaRcle::Graphics::Video::Video(std::string file_name, PlayMode play_mode, RenderMode renderMode) : Material(false, ResourceManager::GetStandartShader()) {
	this->m_playMode = play_mode;
	this->m_file_name = file_name;
	this->m_renderMode = renderMode;

	if (!Load())
		Debug::Error("Video::Constructor() : failed load video \"" + m_file_name + "\", this data structure delete needed!");
}

bool SpaRcle::Graphics::Video::NextFrame() {
	if (m_is_finished) return false;

	double time = glfwGetTime();
	double deltaTime = time - lastTime;

	if (deltaTime >= maxPeriod) {
		lastTime = time;

	ret: if (m_thread_use) goto ret;
		this->m_thread_next = true;

		m_current_frame++;

		m_is_finished = !(m_current_frame < m_count_frames);

		if (m_is_finished) {
			if (this->m_playMode == PlayMode::RepeetOnUse) {
				m_current_frame = 0;
				m_is_finished = false;
			}
			else {
				m_current_frame = m_count_frames - 1;
			}

			this->m_thread_next = false;

			return false;
		}

		this->m_thread_next = false;
	}
	return true;
}

bool SpaRcle::Graphics::Video::Use() {
ret: if (m_thread_next) goto ret;

	this->m_thread_use = true;

	if (this->m_is_finished) {

	} else {
		if (m_renderMode == RenderMode::CalculateInRealTime && m_old_frame != m_current_frame) {
			if (!m_frame_id)
				glGenTextures(1, &m_frame_id);

			m_data = SOIL_load_image_from_memory(
				(const unsigned char*)m_source_frames[m_current_frame].data(),
				m_source_frames[m_current_frame].size(), &m_width, &m_height, &m_channels,
				SOIL_LOAD_AUTO
			);

			glBindTexture(GL_TEXTURE_2D, m_frame_id);

			//!-------------------------------------------------------------------------------------------
			GLuint texParam = GL_CLAMP_TO_BORDER; // GL_REPEAT, GL_CLAMP_TO_BORDER
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texParam);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texParam);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLuint)GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLuint)GL_NEAREST);

			//GL_COMPRESSED_RGB_S3TC_DXT1_EXT
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height,
				0, GL_RGB, GL_UNSIGNED_BYTE, m_data);

			//!-------------------------------------------------------------------------------------------

			SOIL_free_image_data(m_data);

			m_old_frame = m_current_frame;
		}
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_frame_id);
	glUniform1i(glGetUniformLocation(m_fragment_shader->m_ProgramID, "DiffuseMap"), 0);

	this->m_thread_use = false;

	return true;

	/*
	if (m_is_finished) {
		if (this->m_frame_id) {
			glDeleteTextures(1, &m_frame_id);
			m_frame_id = 0;
		}
		glBindTexture(GL_TEXTURE_2D, 0);
		return false;
	}

	m_is_finished = !(m_current_frame < m_count_frames);
	if (m_is_finished && this->m_playMode == PlayMode::RepeetOnUse) {
		m_current_frame = 0;
		m_is_finished = false;
	}

	bool next_frame = GetKey(KeyCode::F) && !m_is_finished; //

	if (next_frame) {
		if (m_renderMode == RenderMode::PreCalculate) {
			if (!m_is_calculate) Calculate();

		}
		else if (m_renderMode == RenderMode::CalculateInRealTime) {
			m_current_frame++;

			if (!m_frame_id)
				glGenTextures(1, &m_frame_id);

			unsigned char* data = SOIL_load_image_from_memory(
				(const unsigned char*)m_source_frames[m_current_frame].data(),
				m_source_frames[m_current_frame].size(), &m_width, &m_height, &m_channels,
				SOIL_LOAD_AUTO
			);

			glBindTexture(GL_TEXTURE_2D, m_frame_id);

			//!-------------------------------------------------------------------------------------------
			GLuint texParam = GL_CLAMP_TO_BORDER; // GL_REPEAT, GL_CLAMP_TO_BORDER
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texParam);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texParam);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLuint)GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLuint)GL_NEAREST);

			//GL_COMPRESSED_RGB_S3TC_DXT1_EXT
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height,
				0, GL_RGB, GL_UNSIGNED_BYTE, data);

			//!-------------------------------------------------------------------------------------------

			SOIL_free_image_data(data);

			m_current_frame++;
		}

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_frame_id);
		glUniform1i(glGetUniformLocation(m_fragment_shader->m_ProgramID, "DiffuseMap"), 0);

		return true;
	}
	else {
		glBindTexture(GL_TEXTURE_2D, 0);
		return false;
	}
	*/
}


bool SpaRcle::Graphics::Video::Calculate() {
	if (m_is_calculate) return false;

	Debug::Log("Video::Calculate() : calculating \"" + this->m_file_name + "\" video...");

	this->m_is_calculate = true;
	return true;
}

bool SpaRcle::Graphics::Video::Load() {
	std::string res = ResourceManager::GetResourceFolder();
	std::string folder = SRString::GetFileNameFromPath(m_file_name);

	folder = res + "\\Cache\\" + folder;

	if (!SRFile::DirExists(folder)) {
		CreateDirectory(SRString::CharsToWchar(folder.c_str()), NULL);
		system((res + "\\Utilites\\ffmpeg\\ffmpeg.exe -loglevel quiet -hide_banner -i " + this->m_file_name + " " + folder + "\\%d.png").c_str());
		system((res + "\\Utilites\\ffmpeg\\ffprobe.exe -hide_banner -v quiet -print_format json -show_format -show_streams " + this->m_file_name + " > " + folder + "\\info.json").c_str());
		//system((res + "\\Utilites\\ffmpeg\\ffprobe.exe -loglevel quiet -hide_banner -i " + m_file_name).c_str());
		// +" -print_format xml \"" + folder + "\\info.asf\""
	}
	//!=======================================================

	std::vector<std::string> files;

	for (const auto& entry : fs::directory_iterator(folder)) {
		if (entry.path().extension() == ".json") {
			std::ifstream is(entry.path());
			std::string line = "";
			std::getline(is, line);
			while (!line.empty()) {
				line = SRString::ReplaceAll(line, " ", "");

				if (line.find("coded_width") != SRMath::size_t_max) {
					if ((line = SRString::GetInBetweenStrings(line, ":", ",")) != "") {
						this->m_width = std::atoi(line.c_str());
					}
				}
				else if (line.find("coded_height") != SRMath::size_t_max) {
					if ((line = SRString::GetInBetweenStrings(line, ":", ",")) != "") {
						this->m_height = std::atoi(line.c_str());
					}
				}

				std::getline(is, line);
			}
		}
		else
		if (entry.path().extension() == ".png")
			files.push_back(entry.path().string());
	}

	this->m_count_frames = files.size();
	m_source_frames.resize(m_count_frames);

	for (size_t t = 0; t < files.size(); t++) {
		size_t index = (size_t)std::atoll(SRString::GetFileNameFromPath(files[t]).c_str());
		int read = 0;
		unsigned char* data = (unsigned char*)SRFile::ReadAllBytes(files[t].c_str(), &read);
		std::vector<unsigned char> vs;
		vs.assign(data, data + read);
		delete[] data;
		this->m_source_frames[index - 1] = vs;
	}
	files.clear();

	if (m_renderMode == RenderMode::PreCalculate) {
		m_source_frames.clear();
	}

	return true;
}

bool SpaRcle::Graphics::Video::Destroy() {
	if (m_is_destroy) return false;

	//for (auto a : m_calculate_frames)
	//	if (a > 0) {
			// opengl context delete glTexture
	//	}
	//this->m_calculate_frames.clear();

	this->m_file_name.clear();
	this->m_source_frames.clear();

	return this->Material::Destroy();
}

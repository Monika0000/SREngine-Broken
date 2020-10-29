#pragma once
#define GLEW_STATIC
#define NOMINMAX
#include <iostream>
#include <GL\glew.h>
#include <map>
#include <SOIL/SOIL.h>
#include <GL\glaux.h>
#pragma comment(lib, "SOIL.lib")

#include <glm/glm.hpp>
#include <imgui.h>

#include <Debug.h>

namespace SpaRcle {
	namespace Graphics {
		struct Image {
			enum class Type {
				BMP, PNG, JPG, TIFF, TGA, UNKNOWN
			};
		public:
			static const std::string TypeToStr(Type type) noexcept {
				switch (type) {
				case Image::Type::BMP:
					return ".bmp";
				case Image::Type::PNG:
					return ".png";
				case Image::Type::JPG:
					return ".jpg";
				case Image::Type::TIFF:
					return ".tif";
				case Image::Type::TGA:
					return ".tga";
				case Image::Type::UNKNOWN:
					return ".unknown";
				default:
					return ".default";
				}
			}
		public:
			unsigned int 
							m_width				= 0, 
							m_height			= 0,
							m_channels			= 0;
			unsigned int	m_imageSize			= 0;
			int				m_format			= 0;
			Type			m_type				= Type::UNKNOWN;
			bool			m_alpha				= false;
			std::string		m_name				= "unnamed";
			unsigned char*	m_data				= nullptr;
		public:
			~Image() {
				m_name.clear();
				m_width			= 0;
				m_height		= 0;
				m_channels		= 0;
				m_type			= Type::UNKNOWN;
				SOIL_free_image_data(m_data);
			}
		public:
			static Image* LoadJPG(const char* path) {
				int width = 0;
				int height = 0;
				int channels = 0;

				unsigned char* image = SOIL_load_image(path, &width, &height, &channels, SOIL_LOAD_AUTO);

				if (!image)
					SpaRcle::Helper::Debug::Error("Image::LoadJPG() : " + std::string(SOIL_last_result()) + "\n\tPath : " + std::string(path));

				Image* jpg = new Image();
				jpg->m_alpha = false;
				jpg->m_data = image;
				jpg->m_type = Image::Type::JPG;
				jpg->m_format = GL_RGB;
				jpg->m_height = height;
				jpg->m_width = width;
				jpg->m_channels = channels;
				jpg->m_imageSize = width * height;

				return jpg;
			}
			static Image* LoadTIFF(const char* path) {
				int width = 0;
				int height = 0;
				int channels = 0;

				unsigned char* image = SOIL_load_image(path, &width, &height, &channels, SOIL_LOAD_AUTO);

				if (!image)
					SpaRcle::Helper::Debug::Error("Image::LoadTIFF() : " + std::string(SOIL_last_result()) + "\n\tPath : " + std::string(path));

				Image* tiff = new Image();
				tiff->m_alpha = true;
				tiff->m_data = image;
				tiff->m_type = Image::Type::TIFF;
				tiff->m_format = GL_RGBA;
				tiff->m_height = height;
				tiff->m_width = width;
				tiff->m_channels = channels;
				tiff->m_imageSize = width * height;

				return tiff;
			}
			static Image* LoadPNG(const char* path) {
				int width = 0;
				int height = 0;
				int channels = 0;

				unsigned char* image = SOIL_load_image(path, &width, &height, &channels, SOIL_LOAD_AUTO);

				//std::cout << &image[0] << std::endl;

				if (!image) {
					SpaRcle::Helper::Debug::Error("Image::LoadPNG() : image is nullptr. \n\tReason : " + std::string(SOIL_last_result()) + "\n\tPath : " + std::string(path));
					return nullptr;
				}

				Image* png = new Image();
				png->m_alpha = true;
				png->m_data = image;
				png->m_type = Image::Type::PNG;
				png->m_format = GL_RGBA;
				png->m_height = height;
				png->m_width = width;
				png->m_channels = channels;
				png->m_imageSize = width * height;

				return png;
			}
		};

		class ResourceManager;
		class Material;
		class Texture {
			friend class ResourceManager;
			enum class Type {
				Diffuse, Normal, Specular, Roughness, Glossiness
			};
			enum class Filter {
				NEAREST = GL_NEAREST, LINEAR = GL_LINEAR, NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
				LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST, NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR, LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR,
			};
		private:
			Texture(Image* image, Type type, Filter filter);
		private:
			Image*				m_image				= nullptr;
		private:
			Type				m_type				= Type::Diffuse;
			Filter				m_filter			= Filter::NEAREST;
		private:
			bool				m_is_calculated		= false;
			bool				m_is_destroyed		= false;
			GLuint				m_id				= 0;
			glm::vec2			m_size				= {0,0};
		private:
			bool Destroy();
			bool Calculate();
		public:
			ImVec2 GetSize() const { return { m_size.x, m_size.y }; }
			const GLuint GetID() noexcept {
				if (!m_is_calculated) Calculate();
				if (m_is_destroyed)
					return 0;
				else
					return m_id;
			}
		};
	}
}


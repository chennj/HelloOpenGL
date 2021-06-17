#include "Texture3D.h"
#include "../stb_image/stb_image.h"

Texture3D::Texture3D(uint32_t color)
{
}

Texture3D::Texture3D(const std::string & left, const std::string & right, const std::string & top, const std::string & bottom, const std::string & front, const std::string & back)
{
	std::vector<std::string> faces
	{
		right,
		left,
		bottom,
		top,
		front,
		back
	};

	LoadCubeMap(faces);
}

Texture3D::~Texture3D()
{
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture3D::LoadCubeMap(std::vector<std::string> faces)
{
	glGenTextures(1, &m_RendererID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);

	stbi_set_flip_vertically_on_load(1);

	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &m_Width, &m_Height, &m_BPP, 0);
		if (data)
		{
			GLenum format;
			if (m_BPP == 1)
				format = GL_RED;
			else if (m_BPP == 3)
				format = GL_RGB;
			else if (m_BPP == 4)
				format = GL_RGBA;

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i].c_str() << std::endl;
			stbi_image_free(data);
		}

	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Texture3D::Bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID));
}

void Texture3D::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}

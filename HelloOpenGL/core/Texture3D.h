#pragma once

#include "Base.h"

#include <iostream>
#include <vector>

class Texture3D
{
private:
	unsigned int m_RendererID;
	int m_Width, m_Height, m_BPP/*bits per pixel*/;

public:
	Texture3D(uint32_t color = 0xffffffff);
	Texture3D(
		const std::string& left, const std::string& right,
		const std::string& top, const std::string& bottom, 
		const std::string& front, const std::string& back);
	~Texture3D();

public:
	void LoadCubeMap(std::vector<std::string> faces);

	void Bind(unsigned int slot = 0)const;
	void Unbind()const;

public:
	inline int GetWidth()const { return m_Width; }
	inline int GetHeight()const { return m_Height; }
	inline int GetRendererID()const { return m_RendererID; }

};
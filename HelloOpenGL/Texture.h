#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "Renderer.h"

class Texture
{
private:
	unsigned int _RendererID;
	std::string _FilePath;
	unsigned char* _LocalBuffer;
	int _Width, _Height, _BPP/*bits per pixel*/;

public:
	Texture(const std::string& filepath);
	~Texture();

public:
	/**
	*
	* slot is that you want to find a texture to now in OpenGL.
	* in like with modern GPUs, you have about 32 texture slots.
	* 
	*/
	void Bind(unsigned int slot = 0)const;
	void Unbind()const;

public:
	inline int GetWidth()const { return _Width; }
	inline int GetHeight()const { return _Height; }
};
#endif

#include "Texture.h"
#include "../stb_image/stb_image.h"

Texture::Texture(uint32_t color)
{
	GLCall(glGenTextures(1, &_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, _RendererID));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color));
}

Texture::Texture(const std::string & filepath)
	:_RendererID(0),_FilePath(filepath),_LocalBuffer(nullptr),_Width(0),_Height(0),_BPP(0)
{
	// we need to make it upside down because OpenGL actually expects our text to kind of 
	// start at the bottom left not the top left, so the bottom left in OpenGL is (0,0), 
	// now typically when we load images like PNG for example stores it in scan lines kind 
	// of from the top of the image to the bottom of the image, so we need to actually flip it on load, 
	// this really does depend on your texture format, so definitely take a look at for our use case 
	// for our PNG here we do need to flip it.
	stbi_set_flip_vertically_on_load(1);

	_LocalBuffer = stbi_load(filepath.c_str(), &_Width, &_Height, &_BPP, 4);

	GLCall(glGenTextures(1, &_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, _RendererID));

	/**
	* 
	* 设置纹理
	* 纹理的坐标系统叫做ST坐标系统，和xy坐标系统一样，s对应x，t对应y
	* 当纹理的大小和渲染屏幕的大小不一致时会出现两种情况： 
	* - 第一种情况：纹理大于渲染屏幕，将会有一部分像素无法映射到屏幕上，对应于GL_TEXTURE_MIN_FILTER
	* - 第二种情况：纹理小于渲染屏幕，没有足够的像素映射到屏幕上，GL_TEXTURE_MAG_FILTER
	* 可以设置GL_NEAREST 和 GL_LINEAR。 
	* - GL_NEAREST：使用纹理中坐标最接近的一个像素的颜色作为需要绘制的像素颜色。 
	* - GL_LINEAR：使用纹理中坐标最接近的若干个颜色，通过加权平均算法得到需要绘制的像素颜色，与GL_NEAREST比较速度较慢。
	* GL_TEXTURE_WRAP_S和GL_TEXTURE_WRAP_T表示超出范围 [0.0, 1.0] 的纹理处理方式
	* 可以设置的值如下：GL_CLAMP_TO_EDGE、GL_REPEAT、GL_MIRRORED_REPEAT。
	* - GL_CLAMP_TO_EDGE：采样纹理边缘，即剩余部分显示纹理临近的边缘颜色值。
	* - GL_REPEAT：重复纹理
	* - GL_MIRRORED_REPEAT:镜像重复
	*/
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _Width, _Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _LocalBuffer));

	//GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (_LocalBuffer)
		stbi_image_free(_LocalBuffer);
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &_RendererID));
}

void Texture::Bind(unsigned int slot) const
{
	// 使GL的0卡槽有效
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));

	GLCall(glBindTexture(GL_TEXTURE_2D, _RendererID));
}

void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

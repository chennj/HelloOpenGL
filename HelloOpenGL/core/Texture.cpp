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
	* ��������
	* ���������ϵͳ����ST����ϵͳ����xy����ϵͳһ����s��Ӧx��t��Ӧy
	* ������Ĵ�С����Ⱦ��Ļ�Ĵ�С��һ��ʱ�������������� 
	* - ��һ����������������Ⱦ��Ļ��������һ���������޷�ӳ�䵽��Ļ�ϣ���Ӧ��GL_TEXTURE_MIN_FILTER
	* - �ڶ������������С����Ⱦ��Ļ��û���㹻������ӳ�䵽��Ļ�ϣ�GL_TEXTURE_MAG_FILTER
	* ��������GL_NEAREST �� GL_LINEAR�� 
	* - GL_NEAREST��ʹ��������������ӽ���һ�����ص���ɫ��Ϊ��Ҫ���Ƶ�������ɫ�� 
	* - GL_LINEAR��ʹ��������������ӽ������ɸ���ɫ��ͨ����Ȩƽ���㷨�õ���Ҫ���Ƶ�������ɫ����GL_NEAREST�Ƚ��ٶȽ�����
	* GL_TEXTURE_WRAP_S��GL_TEXTURE_WRAP_T��ʾ������Χ [0.0, 1.0] ��������ʽ
	* �������õ�ֵ���£�GL_CLAMP_TO_EDGE��GL_REPEAT��GL_MIRRORED_REPEAT��
	* - GL_CLAMP_TO_EDGE�����������Ե����ʣ�ಿ����ʾ�����ٽ��ı�Ե��ɫֵ��
	* - GL_REPEAT���ظ�����
	* - GL_MIRRORED_REPEAT:�����ظ�
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
	// ʹGL��0������Ч
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));

	GLCall(glBindTexture(GL_TEXTURE_2D, _RendererID));
}

void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

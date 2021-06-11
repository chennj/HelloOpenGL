#include "FrameBuffer.h"
#include <iostream>

FrameBuffer::FrameBuffer(const FrameBufferSpecification & spec)
	:m_Specification(spec)
{
	Invalide();
}

FrameBuffer::~FrameBuffer()
{
	GLCall(glDeleteFramebuffers	(1, &m_RendererID));
	GLCall(glDeleteTextures		(1, &m_ColorAttachment));
	GLCall(glDeleteTextures		(1, &m_DepthAttachment));

}

void FrameBuffer::Invalide()
{
	if (m_RendererID)
	{
		GLCall(glDeleteFramebuffers	(1, &m_RendererID));
		GLCall(glDeleteTextures		(1, &m_ColorAttachment));
		GLCall(glDeleteTextures	(1, &m_DepthAttachment));
	}

	if (OPENGL_VERSION >= 4.5)
	{
		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment);
		glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
		glBindTexture(1, m_DepthAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);

	}
	else
	{
		GLCall(glGenFramebuffers(1, &m_RendererID));
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
		
		// Ϊ֡�������������
		GLCall(glGenTextures(1, &m_ColorAttachment));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_ColorAttachment));
		GLCall(glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, m_Specification.Width, m_Specification.Height));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		// �����������ӵ�֡����
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0));

		// Ϊ֡���������Ⱥ�ģ�建��
		GLCall(glGenRenderbuffers(1, &m_DepthAttachment));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_DepthAttachment));
		GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height));
		// ����Ⱥ�ģ�建�帽�ӵ�֡����
		GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthAttachment));
	}

	ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

	// Ҫ��֤���е���Ⱦ�����������������Ӿ�Ч����������Ҫ�ٴμ���Ĭ��֡���壬�����󶨵�0��
	// �ǵ�Ҫ���֡���壬��֤���ǲ��᲻С����Ⱦ�������֡������
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::Bind()
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
	GLCall(glViewport(0, 0, m_Specification.Width, m_Specification.Height));
}

void FrameBuffer::Unbind()
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void FrameBuffer::Resize(uint32_t width, uint32_t height)
{
	if (width == 0 || height == 0)
	{
		std::cout << "Attempted Resize framebuffer to (0,0)" << std::endl;
		return;
	}
	m_Specification.Width = width;
	m_Specification.Height = height;

	Invalide();
}

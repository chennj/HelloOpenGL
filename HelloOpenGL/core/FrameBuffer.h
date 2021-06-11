#pragma once

#include "Base.h"

struct FrameBufferSpecification
{
	uint32_t Width, Height;
	uint32_t Samples = 1;

	bool SwapChainTarget = false;
};

class FrameBuffer
{
private:
	uint32_t m_RendererID = 0;
	uint32_t m_ColorAttachment = 0;
	uint32_t m_DepthAttachment = 0;

	FrameBufferSpecification m_Specification;

public:
	FrameBuffer(const FrameBufferSpecification& spec);
	virtual ~FrameBuffer();

public:
	inline const FrameBufferSpecification& GetSpecification() const { return m_Specification; }
	inline const uint32_t GetColorAttachmentRendererID() const { return m_ColorAttachment; }
	inline const uint32_t GetRendererID() const { return m_RendererID; }

	void Invalide();
	void Bind();
	void Unbind();

	void Resize(uint32_t width, uint32_t height);
};
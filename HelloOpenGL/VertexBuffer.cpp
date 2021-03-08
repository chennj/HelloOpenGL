#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void * data, unsigned int size)
{
	GLCall(glGenBuffers(1, &_RendererID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, _RendererID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &_RendererID));
}

void VertexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, _RendererID));
}

void VertexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

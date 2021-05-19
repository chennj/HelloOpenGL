#ifndef __VERTEX_ARRAY_H_
#define __VERTEX_ARRAY_H_

#include "VertexBuffer.h"

class VertexBufferLayout;

class VertexArray
{
private:
	unsigned int _RendererID;

public:
	VertexArray();
	~VertexArray();

public:
	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
	void Bind() const;
	void Unbind() const;
};
#endif

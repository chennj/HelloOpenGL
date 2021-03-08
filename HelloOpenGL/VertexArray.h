#ifndef __VERTEX_ARRAY__
#define __VERTEX_ARRAY__

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

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

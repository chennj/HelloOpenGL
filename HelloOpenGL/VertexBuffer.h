#ifndef __VERTEX_BUFFER_H__
#define __VERTEX_BUFFER_H_

class VertexBuffer
{
private:
	unsigned int _RendererID;

public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

public:
	void Bind() const;
	void Unbind() const;
};

#endif

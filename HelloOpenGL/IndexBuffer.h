#ifndef __VERTEX_BUFFER_H__
#define __VERTEX_BUFFER_H_

class IndexBuffer
{
private:
	unsigned int _RendererID;
	unsigned int _Count;

public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

public:
	void Bind() const;
	void Unbind() const;

public:
	inline unsigned int GetCount() const {
		return _Count;
	}
};

#endif

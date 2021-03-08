#ifndef __INDEX_BUFFER_H__
#define __INDEX_BUFFER_H__

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

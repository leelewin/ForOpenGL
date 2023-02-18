#pragma once
#include <glad/glad.h>
#include <cstdio>

class VertexBuffer {
public:
	VertexBuffer(const void* data, size_t size);
	~VertexBuffer(); 

	void unBind() const;
	void bind() const;

private:
	unsigned int render_id_;

};
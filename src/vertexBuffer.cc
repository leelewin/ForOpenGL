#include "vertexBuffer.h"

VertexBuffer::VertexBuffer(const void* data, size_t size)
{
	glGenBuffers(1, &render_id_);
	glBindBuffer(GL_ARRAY_BUFFER, render_id_);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

}

VertexBuffer::~VertexBuffer() 
{
	glDeleteBuffers(1, &render_id_);
}

void VertexBuffer::unBind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, render_id_);
}

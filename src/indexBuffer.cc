#include "indexBuffer.h"

IndexBuffer::IndexBuffer(const void* data, unsigned int count)
	: count_(count)
{
	glGenBuffers(1, &render_id_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_id_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count_ * sizeof(unsigned int), data, GL_STATIC_DRAW);

}

IndexBuffer::~IndexBuffer() 
{
	glDeleteBuffers(1, &render_id_);
}

void IndexBuffer::unBind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_id_);
}

unsigned int IndexBuffer::getCount() const
{
	return count_;
}
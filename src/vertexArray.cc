#include "vertexArray.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &render_id_);
	glBindVertexArray(render_id_);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &render_id_);
}

void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	vb.bind();
	auto& elements = layout.getElements();
	for (unsigned int i = 0; i < elements.size(); ++i) {
		// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glVertexAttribPointer(i, elements[i].count, elements[i].type_gl, elements[i].normalized,
		                         layout.getStride(), (void*)elements[i].offset);
		glEnableVertexAttribArray(i);
	}

}

void VertexArray::addIndices(const IndexBuffer& ib)
{
	ib.bind();
}

void VertexArray::bind()
{
	glBindVertexArray(render_id_);
}

void VertexArray::unBind()
{
	glBindVertexArray(0);
}
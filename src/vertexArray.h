#pragma once

#include "vertexBuffer.h"
#include "vertexBufferLayout.h"
#include "indexBuffer.h"

class VertexArray {
public:
	VertexArray();
	~VertexArray();

	void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
	void addIndices(const IndexBuffer& ib);

	void bind();
	void unBind();

private:
	unsigned int render_id_;

};
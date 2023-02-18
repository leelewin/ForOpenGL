#pragma once

#include <vector>
#include <assert.h>

//vertex buffer内存布局
class VertexBufferLayout {
private:
	struct Element {
		unsigned int count;
		unsigned int type_gl;
		unsigned int normalized;
		unsigned int offset;
		static unsigned int getSizeofType(unsigned int type_gl)
		{
			switch (type_gl)
			{
			case GL_FLOAT:
				return 4;
			case GL_UNSIGNED_INT:
				return 4;
			case GL_UNSIGNED_BYTE:
				return 1;
			default:
				break;
			}
			assert(false);
			return 0;
		}
	};

	unsigned int offset_ = 0;
	unsigned int stride_ = 0;
	std::vector<Element> elements_;

public:
	VertexBufferLayout() = default;
	~VertexBufferLayout() = default;

	template<typename T>
	void push(unsigned int count);

	const std::vector<Element> getElements() const { return elements_; }
	unsigned int getStride() const { return stride_; }

};

template<typename T>
inline void VertexBufferLayout::push(unsigned int count)
{
	assert(false);
}

template<>
inline void VertexBufferLayout::push<float>(unsigned int count)
{
	elements_.push_back({count, GL_FLOAT, GL_FALSE, offset_});
	offset_ += count * sizeof(float);
	stride_ = offset_;
}





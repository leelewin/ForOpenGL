#pragma once
#include <glad/glad.h>

class IndexBuffer {
public:
	/// @brief 
	/// @param data 指向indexbuffer的指针
	/// @param count index的数量
	IndexBuffer(const void* data, unsigned int count);
	~IndexBuffer();

	void unBind() const;
	void bind() const;

	unsigned int getCount() const;

private:
	unsigned int render_id_;
	unsigned int count_;

};
#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const std::string& path, const std::string& type, bool filp)
	: render_id_(0), path_(path), type_(type), width_(0), 
	  height_(0), nChannels_(0), texture_data_(nullptr)
{
	if (filp)
		stbi_set_flip_vertically_on_load(true); //纹理颠倒问题
	texture_data_ = stbi_load(path.c_str(), &width_, &height_, &nChannels_, 0);	
	if (!texture_data_)
		perror("failed to load texture");
	glGenTextures(1, &render_id_);
	// std::cout << "render id" << render_id_ << std::endl;
	glBindTexture(GL_TEXTURE_2D, render_id_);
	//设置
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (nChannels_ == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data_);
	else if (nChannels_ == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data_);
	else
		perror("undefine in this nChannels");

	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(texture_data_);
}

Texture::~Texture()
{
	glDeleteTextures(1, &render_id_);
}

//绑定纹理到纹理单元slot
//opengl保证有16个纹理单元供使用
void Texture::bind(unsigned int slot) const
{
	//激活对应的纹理单元
	glActiveTexture(GL_TEXTURE0 + slot);
	//绑定当前纹理到当前激活的纹理单元
	glBindTexture(GL_TEXTURE_2D, render_id_);
}

void Texture::unBind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}































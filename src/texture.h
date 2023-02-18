#pragma once

#include <string>
#include <iostream>
#include <glad/glad.h>

//
//
class Texture {
public:
	//type texture_diffuse/texture_specular
	//TODO: type change to enum
	Texture(const std::string& path, const std::string& type, bool filp = false);
	Texture(const Texture& other) = delete;
	~Texture();

	void bind(unsigned int slot = 0) const;
	void unBind() const;

	std::string getTexType() const { return type_; }

private:
	unsigned int render_id_;
	std::string path_;
	std::string type_;
	int width_;
	int height_;
	int nChannels_;
	unsigned char* texture_data_;

};




























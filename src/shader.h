#pragma once

#include <string>
#include <glad/glad.h>

class ShaderProgram {
public:
	ShaderProgram(const std::string& vertexPath, const std::string& fragPath);
	~ShaderProgram();

	void bind() const;
	void unbind() const;

	int getUniformLocation(const std::string& name);
	void setUniform3f(const std::string& name, float v0, float v1, float v2);
	void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void setUniform1i(const std::string& name, int v0);
	void setUniform1f(const std::string& name, float v0);
	void setUniformMat4fv(const std::string& name, float* value);

private:
	unsigned int createShaderProgram(const std::string &vertexShaderSource, 
	                                       const std::string &fragShaderSource);
	unsigned int compileShader(unsigned int shaderType, const char* shaderSource);

private:
	unsigned int render_id_;


};

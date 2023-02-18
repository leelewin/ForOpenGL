#include "shader.h"
#include <sstream>
#include <fstream>
#include <iostream>

ShaderProgram::ShaderProgram(const std::string& vertexPath, const std::string& fragPath)
	: render_id_(0)
{
	std::string vertexShaderSource;
	std::string fragShaderSource;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try 
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();		
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexShaderSource   = vShaderStream.str();
        fragShaderSource = fShaderStream.str();		
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
	render_id_ = createShaderProgram(vertexShaderSource, fragShaderSource);

}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(render_id_);
}

void ShaderProgram::bind() const
{
	glUseProgram(render_id_);
}

void ShaderProgram::unbind() const
{
	glUseProgram(0);
}

void ShaderProgram::setUniform3f(const std::string& name, float v0, float v1, float v2)
{
	glUniform3f(getUniformLocation(name), v0, v1, v2);
}

void ShaderProgram::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	glUniform4f(getUniformLocation(name), v0, v1, v2, v3);
}

//设置着色器采样器属于那个纹理单元v0
void ShaderProgram::setUniform1i(const std::string& name, int v0)
{
	glUniform1i(getUniformLocation(name), v0);
}

void ShaderProgram::setUniform1f(const std::string& name, float v0)
{
	glUniform1f(getUniformLocation(name), v0);
}

void ShaderProgram::setUniformMat4fv(const std::string& name, float* value)
{
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, value);
}

int ShaderProgram::getUniformLocation(const std::string& name)
{
	int location = glGetUniformLocation(render_id_, name.c_str());
	if (location == -1) {
		std::cout << "error : " << name << " not exist" << std::endl; 
	}
	return location;

}

unsigned int ShaderProgram::createShaderProgram(const std::string &vertexShaderSource, 
										const std::string &fragShaderSource)
{
	unsigned int shader_program;
	shader_program = glCreateProgram();
	unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShaderSource.c_str());
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragShaderSource.c_str());
	glAttachShader(shader_program, vs);
	glAttachShader(shader_program, fs);
	glLinkProgram(shader_program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return shader_program;

}

unsigned int ShaderProgram::compileShader(unsigned int shaderType, const char* shaderSource)
{
	unsigned int shader;
	shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderSource, NULL); //string ？？
	glCompileShader(shader);

	//TODO
	//error check

	return shader;
}


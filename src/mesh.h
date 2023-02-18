#pragma once

#include "vertexBuffer.h"
#include "indexBuffer.h"
#include "vertexArray.h"
#include "shader.h"
// #include "util.h"
#include "texture.h"

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <string>

struct Vertex {
	//内存正好是对齐的
	glm::vec3 pos; 
	glm::vec3 normal;
	glm::vec2 tex_coords;
};

// Mesh类表示了一系列节点及其属性组成的网格，可以调用draw进行渲染绘制
// Mesh作为Module的组成部分
class Mesh {
public:
	//vs: vertex vector，default need include pos、normal、texcoord
	//is: indic vector
	//texs: texture vector
	Mesh(const std::vector<Vertex> &vs, const std::vector<unsigned int> &is, std::vector<std::shared_ptr<Texture>>& texs);
	Mesh(const Mesh& other) = delete;
	~Mesh() = default;

	void draw(ShaderProgram& shader);

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<std::shared_ptr<Texture>> textures;

private:
	VertexArray va_;
	VertexBuffer vb_;
	IndexBuffer ib_;

	void setupMesh();

};




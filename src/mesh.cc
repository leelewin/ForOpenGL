#include "mesh.h"

Mesh::Mesh(const std::vector<Vertex> &vs, const std::vector<unsigned int> &is, std::vector<std::shared_ptr<Texture>>& texs)
	: vertices(vs), indices(is), textures(texs), 
	  vb_(&vertices[0], vertices.size() * sizeof(Vertex)),
	  ib_(&indices[0], indices.size()),
	  va_()
{
	std::cout << "vs_size:" << vertices.size() << " is_size:" << indices.size() << std::endl;
	setupMesh();
}

// Mesh::~Mesh()
// {
// }

void Mesh::draw(ShaderProgram& shader)
{
	/*
	frag shader中sampler2D的命名约定:
	每个漫反射纹理被命名为texture_diffuseN
	每个镜面反射纹理被命名为texture_specularN
	其中，N的范围是1到纹理采样器最大允许的数字
	*/
	//texture bind

	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	for (unsigned int i = 0; i < textures.size(); ++i) {
		// std::cout << "before" << glGetError() << std::endl;
		textures[i]->bind(i);
		// std::cout << "after" << glGetError() << std::endl;
		//获取纹理序号
		std::string number;
		std::string name = textures[i]->getTexType();
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular") 
			number = std::to_string(specularNr++);
		
		shader.bind();
		shader.setUniform1i(name + number, i);

	}
	//TODO

	va_.bind();
	glDrawElements(GL_TRIANGLES, ib_.getCount(), GL_UNSIGNED_INT, 0);
	va_.unBind();
}

void Mesh::setupMesh()
{
	VertexBufferLayout layout = VertexBufferLayout();
	//顶点位置
	layout.push<float>(3);
	//法线位置
	layout.push<float>(3);
	//顶点纹理坐标
	layout.push<float>(2);
	va_.addBuffer(vb_, layout);
	va_.addIndices(ib_);
	va_.unBind();

}
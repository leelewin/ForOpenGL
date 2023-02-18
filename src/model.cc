#include "model.h"
#include <iostream>

Model::Model(const char* path)
{
	loadModel(std::string(path));
}

void Model::draw(ShaderProgram& shader)
{
	for (auto& mesh : meshes_) {
		mesh->draw(shader);
	}
}

void Model::loadModel(const std::string& path)
{
	Assimp::Importer importer;
	//如果模型不包含法向量，则为每个顶点创建法线
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
	if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
		std::cout << "ERROR:assimp" << importer.GetErrorString() << std::endl;
		return;
	}
	directory_ = path.substr(0, path.find_last_of('/'));
	processNode(scene->mRootNode, scene);
}

//迭代处理
void Model::processNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		processMesh(mesh, scene);
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++) { //mNumChildren=0时就退出
		processNode(node->mChildren[i], scene);
	}
}

void Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<std::shared_ptr<Texture>> textures;

	// all vertices in this mesh
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		glm::vec3 vec3f;
		//pos
		vec3f.x = mesh->mVertices[i].x;
		vec3f.y = mesh->mVertices[i].y;
		vec3f.z = mesh->mVertices[i].z;
		vertex.pos = vec3f;

		//normal
		vec3f.x = mesh->mNormals[i].x;
		vec3f.y = mesh->mNormals[i].y;
		vec3f.z = mesh->mNormals[i].z;
		vertex.normal = vec3f;

		//texcoord
		glm::vec2 vec2f;
		if (mesh->HasTextureCoords(0)) {
			vec2f.x = mesh->mTextureCoords[0][i].x;
			vec2f.y = mesh->mTextureCoords[0][i].y;
			vertex.tex_coords = vec2f;
		} else {
			std::cout << "Mesh not have texcood, and set all (0, 0)" << std::endl;
			vertex.tex_coords = glm::vec2(0.0f, 0.0f);
		}
		// vertex.tex_coords = vec2f;
		vertices.push_back(vertex);
	}

	//all indices in this mesh
	for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; ++j) {
			indices.push_back(face.mIndices[j]);
		}
	}

	//all texcood in this mesh
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	auto diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");

	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	auto specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

	meshes_.push_back(std::unique_ptr<Mesh>(new Mesh(vertices, indices, textures)));

}

std::vector<std::shared_ptr<Texture>> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type,
                                                      std::string pName)
{
	std::vector<std::shared_ptr<Texture>> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i) {
		aiString str;
		mat->GetTexture(type, i, &str);
		std::string texPath = directory_ + '/' + str.C_Str();
		textures.push_back(std::shared_ptr<Texture>(new Texture(texPath, pName)));

	}
	return textures;
}


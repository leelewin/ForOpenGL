#pragma once

#include "shader.h"
#include "mesh.h"
#include <vector>
#include <memory>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//表示场景中一个模型，该模型可能由多个mesh组成
//todo：
//保留mesh的父子关系
class Model {
public:
	Model(const char* path);

	void draw(ShaderProgram& shader);
private:
	// std::vector<Mesh> meshes_;
	std::vector<std::unique_ptr<Mesh>> meshes_;
	std::string directory_;

	void loadModel(const std::string& path);
	void processNode(aiNode* node, const aiScene* scene);
	void processMesh(aiMesh* mesh, const aiScene* scene);

	std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial* mat, aiTextureType type, //采用ptr的原因是texture析构会导致id被删除
                                                      std::string pName);


};




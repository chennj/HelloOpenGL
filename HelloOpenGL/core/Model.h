#pragma once

#include "Base.h"
#include "Shader.h"
#include "Mesh.h"

#include <glm/glm.hpp>
#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
public:
	// model data -------------------------------------------
	// stores all the textures loaded so far, optimization to 
	// make sure textures aren't loaded more than once.
	std::vector<AssimpTexture> m_TexturesLoaded;	
	std::vector<Mesh>	m_Meshes;
	std::string			m_Directory;
	bool				m_GammaCorrection;
	// ------------------------------------------------------

public:
	/*  º¯Êý   */
	Model(std::string const &path, bool gamma = false);
private:
	/*  º¯Êý   */
	void LoadModel(std::string const & path);
	void ProcessNode(aiNode *node, const aiScene *scene);
	Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<AssimpTexture> LoadMaterialTextures(aiMaterial *mat, aiTextureType type,
		std::string typeName);
public:
	void Draw(Ref<Shader>& shader);
	void Draw1(Ref<Shader>& shader);
};
#pragma once

#include "Base.h"
#include "Shader.h"

#include <glm/glm.hpp>
#include <string>
#include <vector>

struct AssimpVertex
{
	// 顶点位置
	glm::vec3 Position;
	// 每一个点的法向量
	glm::vec3 Normal;
	// 纹理坐标
	glm::vec2 TexCoords;
	// 切线
	glm::vec3 Tangent;
	// 双切线
	glm::vec3 Bitangent;
};

struct AssimpTexture
{
	unsigned int Id;
	std::string Type;
	std::string Path;
};

class Mesh
{
public:
	/*  渲染数据  */
	unsigned int VAO, VBO, IBO;

public:
	Mesh(float vertexs[], unsigned int indices[], std::vector<AssimpTexture> textures);
	Mesh(std::vector<AssimpVertex> vertices, std::vector<unsigned int> indices, std::vector<AssimpTexture> textures);
	~Mesh();

public:
	/*  网格数据  */
	std::vector<AssimpVertex> m_Vertices;
	std::vector<unsigned int> m_Indices;
	std::vector<AssimpTexture> m_Textures;

private:
	/*  函数  */
	void SetupMesh();

public:
	/*  函数  */
	void Draw(Ref<Shader>& shader);
	void Draw1(Ref<Shader>& shader);
};
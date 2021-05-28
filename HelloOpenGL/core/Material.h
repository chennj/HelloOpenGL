#pragma once

#include "Base.h"
#include "Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Material
{
private:
	Ref<Shader> m_Shader;

	glm::vec3 m_Ambient = { 0.2f, 0.3f, 0.31f };
	glm::vec3 m_Diffuse = { 0.9f, 0.9f, 0.9f };
	glm::vec3 m_Specular = { 0.5f, 0.5f, 0.5f };
	float m_Shininess = 32.0f;

public:
	Material(const Ref<Shader>& shader);
	Material(const Ref<Shader>& shader, glm::vec3& ambient, glm::vec3& diffuse, glm::vec3& specular, float shininess);
	~Material();

public:
	inline glm::vec3	GetAmbient	()	{ return m_Ambient; }
	inline glm::vec3	GetDiffuse	()	{ return m_Diffuse; }
	inline glm::vec3	GetSpecular	()	{ return m_Specular; }
	inline float		GetShininess()	{ return m_Shininess; }

	void SetAmbient		(glm::vec3& value,const std::string& name= "material.ambient");
	void SetDiffuse		(glm::vec3& value,const std::string& name= "material.diffuse");
	void SetSpecular	(glm::vec3& value,const std::string& name= "material.specular");
	void SetShininess	(float value, const std::string& name = "material.shininess");

	void SetAmbient		(const std::string& name = "material.ambient");
	void SetDiffuse		(const std::string& name = "material.diffuse");
	void SetSpecular	(const std::string& name = "material.specular");
	void SetShininess	(const std::string& name = "material.shininess");
};
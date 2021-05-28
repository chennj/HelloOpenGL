#include "Material.h"

Material::Material(const Ref<Shader>& shader)
	:m_Shader(shader)
{
}

Material::Material(const Ref<Shader>& shader, glm::vec3& ambient, glm::vec3& diffuse, glm::vec3& specular, float shininess)
	:m_Shader(shader),m_Ambient(ambient),m_Diffuse(diffuse),m_Specular(specular),m_Shininess(shininess)
{
}

Material::~Material()
{
}

void Material::SetAmbient(glm::vec3 & value, const std::string & name)
{
	m_Ambient = value;
	m_Shader->SetUniform3f(name, value);
}

void Material::SetDiffuse(glm::vec3 & value, const std::string & name)
{
	m_Diffuse = value;
	m_Shader->SetUniform3f(name, value);
}

void Material::SetSpecular(glm::vec3 & value, const std::string & name)
{
	m_Specular = value;
	m_Shader->SetUniform3f(name, value);
}

void Material::SetShininess(float value, const std::string & name)
{
	m_Shininess = value;
	m_Shader->SetUniform1f(name, value);
}

void Material::SetAmbient(const std::string & name)
{
	SetAmbient(m_Ambient, name);
}

void Material::SetDiffuse(const std::string & name)
{
	SetDiffuse(m_Diffuse, name);
}

void Material::SetSpecular(const std::string & name)
{
	SetSpecular(m_Specular, name);
}

void Material::SetShininess(const std::string & name)
{
	SetShininess(m_Shininess, name);
}

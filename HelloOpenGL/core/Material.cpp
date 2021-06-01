#include "Material.h"

Material::Material(const Ref<Shader>& shader, const std::string& name)
	:m_Shader(shader), m_Name(name)
{
}

Material::~Material()
{
}

void Material::SetAmbient(glm::vec3 & value, const std::string & name)
{
	m_Shader->SetUniform3f(name, value);
}

void Material::SetDiffuse(glm::vec3 & value, const std::string & name)
{

	m_Shader->SetUniform3f(name, value);
}

void Material::SetSpecular(glm::vec3 & value, const std::string & name)
{

	m_Shader->SetUniform3f(name, value);
}

void Material::SetAmbient(glm::vec3 & value)
{
	SetAmbient(value, m_Name + ".ambient");
}

void Material::SetDiffuse(glm::vec3 & value)
{
	SetDiffuse(value, m_Name + ".diffuse");
}

void Material::SetSpecular(glm::vec3 & value)
{
	SetSpecular(value, m_Name + ".specular");
}

void Material::SetAmbient(Ref<Texture>& tex, int slot, const std::string & name)
{
	tex->Bind(slot);
	m_Shader->SetUniform1i(name, slot);
}

void Material::SetDiffuse(Ref<Texture>& tex, int slot, const std::string & name)
{
	tex->Bind(slot);
	m_Shader->SetUniform1i(name, slot);
}

void Material::SetSpecular(Ref<Texture>& tex, int slot, const std::string & name)
{
	tex->Bind(slot);
	m_Shader->SetUniform1i(name, slot);
}

void Material::SetAmbient(Ref<Texture>& tex, int slot)
{
	SetAmbient(tex, slot, m_Name + ".ambient");
}

void Material::SetDiffuse(Ref<Texture>& tex, int slot)
{
	SetDiffuse(tex, slot, m_Name + ".diffuse");
}

void Material::SetSpecular(Ref<Texture>& tex, int slot)
{
	SetSpecular(tex, slot, m_Name + ".specular");
}

void Material::SetShininess(float value, const std::string & name)
{
	m_Shader->SetUniform1f(name, value);
}

void Material::SetShininess(float value)
{
	SetShininess(value, m_Name + ".shininess");
}

void Material::SetPosition(glm::vec3 & value, const std::string & name)
{
	m_Shader->SetUniform3f(name, value);
}

void Material::SetPosition(glm::vec3 & value)
{
	SetPosition(value, m_Name + ".position");
}

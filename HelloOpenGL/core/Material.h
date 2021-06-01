#pragma once

#include "Base.h"
#include "Shader.h"

#include "../core/Texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Material
{
private:
	Ref<Shader> m_Shader;
	std::string m_Name;

public:
	Material(const Ref<Shader>& shader, const std::string& name="material");
	~Material();

public:
	void SetAmbient		(glm::vec3& value,const std::string& name);
	void SetDiffuse		(glm::vec3& value,const std::string& name);
	void SetSpecular	(glm::vec3& value,const std::string& name);

	void SetAmbient		(glm::vec3& value);
	void SetDiffuse		(glm::vec3& value);
	void SetSpecular	(glm::vec3& value);

	void SetAmbient		(Ref<Texture>& tex, int slot, const std::string& name);
	void SetDiffuse		(Ref<Texture>& tex, int slot, const std::string& name);
	void SetSpecular	(Ref<Texture>& tex, int slot, const std::string& name);

	void SetAmbient		(Ref<Texture>& tex, int slot);
	void SetDiffuse		(Ref<Texture>& tex, int slot);
	void SetSpecular	(Ref<Texture>& tex, int slot);

	void SetShininess	(float value, const std::string& name);
	void SetShininess	(float value);

	void SetPosition	(glm::vec3& value, const std::string& name);
	void SetPosition	(glm::vec3& value);
};
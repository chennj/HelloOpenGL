#shader vertex
#version 330 core

layout(location = 0) in vec4 position;	//立方体每个面的坐标
layout(location = 1) in vec3 normal;	//立方体每个面的法向量
layout(location = 2) in vec2 texCoord;	//立方体每个面的纹理坐标

out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 v_FragPosition;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

void main()
{
	gl_Position = u_Proj * u_View  * u_Model * position;
	v_TexCoord = texCoord;
	v_FragPosition = vec3(u_Model * position);	// 转到世界坐标
	v_Normal = mat3(transpose(inverse(u_Model))) * normal;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 FragColor;

in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_FragPosition;

uniform sampler2D u_Texture;
uniform sampler2D u_Background;

uniform vec3 lightPosition;
uniform vec3 viewPosition;
uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
	// 规则化法向量
	vec3 normal = normalize(v_Normal);

	// Ambinet
	float ambinetStrength = 0.1f;
	vec3 ambinet = ambinetStrength * lightColor;

	// Diffuse
	vec3 lightDirection = normalize(lightPosition - v_FragPosition);	// vector pointing to the light, from fragment position
	vec3 diffuse = max(dot(normal, lightDirection), 0.0) * lightColor;

	// Specular
	float specularStrength = 0.5f;
	vec3 viewDirection = normalize(viewPosition - v_FragPosition);
	vec3 reflectDirection = reflect(-lightDirection, normal);
	vec3 specular = specularStrength * pow(max(dot(viewDirection, reflectDirection), 0.0), 64) * lightColor;

	// 冯氏光照模型
	// (ambinet + diffuse + specular) * objectColor * textureColor
	vec3 result = (ambinet + diffuse + specular) * objectColor;
	FragColor = vec4(result, 1.0) * texture(u_Texture, v_TexCoord) * texture(u_Background, v_TexCoord);
};
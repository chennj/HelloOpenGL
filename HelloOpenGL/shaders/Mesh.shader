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

struct Material {
	sampler2D diffuse1;
	sampler2D specular1;
	float shininess;
};

// 平行光源
struct LightDirectional {
	vec3 position;
	vec3 color;
	vec3 directionToLight;
};

// 点光源
struct LightPoint {
	vec3 position;
	vec3 color;
	vec3 directionToLight;

	float constant;
	float linear;
	float quadratic;
};

// 聚光灯
struct LightSpot {
	vec3 position;
	vec3 color;
	vec3 directionToLight;

	float constant;
	float linear;
	float quadratic;

	float cutoff;
	float cutoffouter;
};

uniform Material material;
uniform LightDirectional lightD;
uniform LightPoint lightP0, lightP1, lightP2, lightP3;
uniform LightSpot lightS;

uniform vec3 cameraPosition;

vec3 CalcLightDirectional(LightDirectional light, vec3 normal, vec3 dirToCamera)
{
	// diffuse
	float diffuseIntensity = max(dot(light.directionToLight, normal), 0);
	vec3 diffuseColor = diffuseIntensity * light.color * texture(material.diffuse1, v_TexCoord).rgb;

	// specular
	vec3 R = normalize(reflect(-light.directionToLight, normal));
	float specularIntensity = pow(max(dot(R, dirToCamera), 0), material.shininess);
	vec3 specularColor = specularIntensity * light.color * texture(material.specular1, v_TexCoord).rgb;

	return diffuseColor + specularColor;
}

vec3 CalcLightPoint(LightPoint light, vec3 normal, vec3 dirToCamera)
{
	// attenuation
	float dist = length(light.position - v_FragPosition);
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	// diffuse
	float diffuseIntensity = max(dot(normalize(light.position - v_FragPosition), normal), 0) * attenuation;
	vec3 diffuseColor = diffuseIntensity * light.color * texture(material.diffuse1, v_TexCoord).rgb;

	// specular
	vec3 R = normalize(reflect(-normalize(light.position - v_FragPosition), normal));
	float specularIntensity = pow(max(dot(R, dirToCamera), 0), material.shininess) * attenuation;
	vec3 specularColor = specularIntensity * light.color * texture(material.specular1, v_TexCoord).rgb;

	return diffuseColor + specularColor;
}

vec3 CalcLightSpot(LightSpot light, vec3 normal, vec3 dirToCamera)
{
	// attenuation
	float dist = length(light.position - v_FragPosition);
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	// diffuse
	float diffuseIntensity = max(dot(normalize(light.position - v_FragPosition), normal), 0) * attenuation;
	vec3 diffuseColor = diffuseIntensity * light.color * texture(material.diffuse1, v_TexCoord).rgb;

	// specular
	vec3 R = normalize(reflect(-normalize(light.position - v_FragPosition), normal));
	float specularIntensity = pow(max(dot(R, dirToCamera), 0), material.shininess) * attenuation;
	vec3 specularColor = specularIntensity * light.color * texture(material.specular1, v_TexCoord).rgb;

	// 切光角的cos值
	float cosTheta = dot(normalize(v_FragPosition - light.position), -1.0f * light.directionToLight);
	float spotRatio;
	if (cosTheta > light.cutoff)
	{	//inside
		spotRatio = 1.0f;
	}
	else if (cosTheta > light.cutoffouter)
	{	//between inside and outside, is Smooth edges in the middle.
		spotRatio = 1.0f - (cosTheta - light.cutoff) / (light.cutoffouter - light.cutoff);
	}
	else
	{	//outside
		spotRatio = 0.0f;
	}

	return (diffuseColor + specularColor) * spotRatio;
}

void main()
{
	vec3 finalResult = vec3(0, 0, 0);
	vec3 uNormal = normalize(v_Normal);
	vec3 directionToCamera = normalize(cameraPosition - v_FragPosition);

	finalResult += CalcLightDirectional(lightD, uNormal, directionToCamera);
	finalResult += CalcLightPoint(lightP0, uNormal, directionToCamera);
	finalResult += CalcLightPoint(lightP1, uNormal, directionToCamera);
	finalResult += CalcLightPoint(lightP2, uNormal, directionToCamera);
	finalResult += CalcLightPoint(lightP3, uNormal, directionToCamera);
	finalResult += CalcLightSpot(lightS, uNormal, directionToCamera);

	FragColor = vec4(finalResult, 1.0);
};

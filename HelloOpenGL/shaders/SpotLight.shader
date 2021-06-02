#shader vertex
#version 330 core

layout(location = 0) in vec4 position;	//������ÿ���������
layout(location = 1) in vec3 normal;	//������ÿ����ķ�����
layout(location = 2) in vec2 texCoord;	//������ÿ�������������

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
	v_FragPosition = vec3(u_Model * position);	// ת����������
	v_Normal = mat3(transpose(inverse(u_Model))) * normal;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 FragColor;

in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_FragPosition;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light {
	vec3 position;
	vec3 color;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float cutoff;
	float cutoffouter;
};

uniform Material material;
uniform Light light;

uniform sampler2D u_Texture;
uniform sampler2D u_Background;

uniform vec3 viewPosition;

void main()
{
	// ������
	vec3 norm = normalize(v_Normal);

	// ����⣨lightDir��������⣨reflectDir���������viewDir��
	vec3 lightDir = normalize(light.position - v_FragPosition);
	vec3 reflectDir = reflect(-lightDir, norm);
	vec3 viewDir = normalize(viewPosition - v_FragPosition);

	// ambient
	vec3 ambient = light.ambient * texture(material.diffuse, v_TexCoord).rgb * light.color;

	// diffuse 
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, v_TexCoord).rgb * light.color;

	// specular
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, v_TexCoord).rgb * light.color ;

	// emission
	vec3 emission	= (texture(u_Texture, v_TexCoord) * texture(u_Background, v_TexCoord)).rgb;
	emission = vec3(0.0, 0.0, 0.0);

	// �й�ǵ�cosֵ
	float cosTheta = dot(normalize(v_FragPosition - light.position), -1.0f * light.direction);
	float spotRatio;
	if (cosTheta > light.cutoff)
	{	//inside
		spotRatio = 1.0f;
	}
	else if (cosTheta > light.cutoffouter)
	{	//between inside and outside, is Smooth edges in the middle.
		spotRatio = 1.0f - (cosTheta - light.cutoff)/(light.cutoffouter - light.cutoff);
	}
	else
	{	//outside
		spotRatio = 0.0f;
	}

	vec3 result = ambient + (diffuse + specular) * spotRatio + emission;
	FragColor = vec4(result, 1.0);

	//if (cosTheta > light.cutoff)
	//{	//inside
	//	vec3 result = ambient + diffuse + specular + emission;
	//	FragColor = vec4(result, 1.0);
	//}
	//else
	//{	//outside
	//	vec3 result = ambient;
	//	FragColor = vec4(result, 1.0);
	//}

};
#shader vertex
#version 330 core

layout(location = 0) in vec4 position;	//立方体每个面的坐标
layout(location = 1) in vec3 normal;	//立方体每个面的法向量

out vec3 v_Normal;
out vec3 v_Position;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

void main()
{
	gl_Position = u_Proj * u_View  * u_Model * position;

	v_Position = vec3(u_Model * position);
	v_Normal = mat3(transpose(inverse(u_Model))) * normal;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 FragColor;

in vec3 v_Position;
in vec3 v_Normal;

uniform vec3 cameraPos;
// 3D纹理采样
uniform samplerCube textureIndex;
// 2D纹理采样
//uniform sampler2D textureIndex;

void main()
{
	// 反射
	//vec3 I = normalize(v_Position - cameraPos);
	//vec3 R = reflect(I, normalize(v_Normal));
	//FragColor = vec4(texture(textureIndex, R).rgb, 1.0f);

	// 折射
	float ratio = 1.00 / 1.52;
	vec3 I = normalize(v_Position - cameraPos);
	vec3 R = refract(I, normalize(v_Normal), ratio);
	FragColor = vec4(texture(textureIndex, R).rgb, 1.0f);
};
#version 330 core

layout(location = 0) in vec4 position;	//立方体每个面的坐标
layout(location = 1) in vec2 texCoord;	//立方体每个面的纹理坐标

out vec2 v_TexCoord;
out vec3 v_FragPosition;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

void main()
{
	gl_Position = u_Proj * u_View  * u_Model * position;
	v_TexCoord = texCoord;
	v_FragPosition = vec3(u_Model * position);	// 转到世界坐标
};

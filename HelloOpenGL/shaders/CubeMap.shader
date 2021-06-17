#shader vertex
#version 330 core

layout(location = 0) in vec4 position;	//立方体每个面的坐标
layout(location = 1) in vec3 normal;	//立方体每个面的法向量
layout(location = 2) in vec2 texCoord;	//立方体每个面的纹理坐标

out vec2 v_TexCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

void main()
{
	gl_Position = u_Proj * u_View  * u_Model * position;
	v_TexCoord = texCoord;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 FragColor;

in vec2 v_TexCoord;

uniform sampler2D textureIndex;

void main()
{
	FragColor = texture(textureIndex, v_TexCoord);
};
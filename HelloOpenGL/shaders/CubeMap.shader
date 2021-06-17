#shader vertex
#version 330 core

layout(location = 0) in vec4 position;	//������ÿ���������
layout(location = 1) in vec3 normal;	//������ÿ����ķ�����
layout(location = 2) in vec2 texCoord;	//������ÿ�������������

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
#shader vertex
#version 330 core

layout(location = 0) in vec4 position;	//������ÿ���������
layout(location = 1) in vec2 texCoord;	//������ÿ�������������

out vec2 v_TexCoord;
out vec3 v_FragPosition;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

void main()
{
	gl_Position = u_Proj * u_View  * u_Model * position;
	v_TexCoord = texCoord;
	v_FragPosition = vec3(u_Model * position);	// ת����������
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 FragColor;

in vec2 v_TexCoord;
in vec3 v_FragPosition;

uniform sampler2D textureIndex;
uniform bool drawFrame;

void main()
{
	if (drawFrame)
	{
		FragColor = vec4(1.00, 0.28, 0.26, 1.0);
	}
	else
	{
		FragColor = texture(textureIndex, v_TexCoord);
	}
};


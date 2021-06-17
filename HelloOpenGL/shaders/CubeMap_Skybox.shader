#shader vertex
#version 330 core

layout(location = 0) in vec3 position;	//立方体每个面的坐标

out vec3 v_TexCoord;

uniform mat4 u_View;
uniform mat4 u_Proj;

void main()
{
	gl_Position = u_Proj * u_View  * vec4(position,1.0f);
	v_TexCoord = position;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 FragColor;

in vec3 v_TexCoord;

uniform samplerCube skybox;

void main()
{
	FragColor = texture(skybox, v_TexCoord);
};
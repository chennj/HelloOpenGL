#shader vertex
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

#shader fragment
#version 330 core

layout(location = 0) out vec4 FragColor;

in vec2 v_TexCoord;
in vec3 v_FragPosition;

uniform sampler2D textureIndex;
uniform bool depthVisual;

float near = 0.1;
float far = 100.0;

float LinearizeDepth(float depth)
{
	float z = depth * 2.0 - 1.0; // back to NDC 
	return (2.0 * near * far) / (far + near - z * (far - near));
}
//-----------------------------------------
// gl_FragCoord -- 用于存储深度缓冲值的内建向量；
//-----------------------------------------
void main()
{
	if (depthVisual) 
	{
		//FragColor = vec4(vec3(gl_FragCoord.z), 1.0);						//z线性变化
		FragColor = vec4(vec3(LinearizeDepth(gl_FragCoord.z) / far), 1.0);	//z非线性变化
	}
	else
	{
		FragColor = texture(textureIndex, v_TexCoord);
	}
};


#shader vertex
#version 330 core

layout(location = 0) in vec4 position;	//立方体每个面的坐标
layout(location = 1) in vec2 texCoord;	//立方体每个面的纹理坐标

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
	vec4 texColor = texture(textureIndex, v_TexCoord);
	// -----------------------------------------------------
	// 如果主程序使用了
	// glEnable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// 也可以不要下面的代码了，当然使用它，颜色管理有时候会更灵活、精确。
	if (texColor.a < 0.1)
		discard;
	// -----------------------------------------------------
	FragColor = texColor;
};


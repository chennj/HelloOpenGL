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
// gl_FragCoord -- ���ڴ洢��Ȼ���ֵ���ڽ�������
//-----------------------------------------
void main()
{
	if (depthVisual) 
	{
		//FragColor = vec4(vec3(gl_FragCoord.z), 1.0);						//z���Ա仯
		FragColor = vec4(vec3(LinearizeDepth(gl_FragCoord.z) / far), 1.0);	//z�����Ա仯
	}
	else
	{
		FragColor = texture(textureIndex, v_TexCoord);
	}
};

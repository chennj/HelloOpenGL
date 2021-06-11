#shader vertex
#version 330 core

layout(location = 0) in vec4 position;	//������ÿ���������
layout(location = 1) in vec2 texCoord;	//������ÿ�������������

out vec2 v_TexCoord;
out vec3 v_FragPosition;

void main()
{
	gl_Position = position;
	v_TexCoord = texCoord;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 FragColor;

in vec2 v_TexCoord;
in vec3 v_FragPosition;

uniform sampler2D textureIndex;

void main()
{
	//FragColor = texture(textureIndex, v_TexCoord);

	// ����
	//FragColor = vec4(vec3(1.0 - texture(textureIndex, v_TexCoord)), 1.0);

	// �Ҷ�
	//FragColor = texture(textureIndex, v_TexCoord);
	//float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0;
	//FragColor = vec4(average, average, average, 1.0);

	// ��Ȩ�Ҷ�
	//FragColor = texture(textureIndex, v_TexCoord);
	//float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
	//FragColor = vec4(average, average, average, 1.0);

	// ��Ч������������
	//const float offset = 1.0 / 300.0;
	//vec2 offsets[9] = vec2[](
	//	vec2(-offset, offset),	// ����
	//	vec2(0.0f, offset),		// ����
	//	vec2(offset, offset),	// ����
	//	vec2(-offset, 0.0f),	// ��
	//	vec2(0.0f, 0.0f),		// ��
	//	vec2(offset, 0.0f),		// ��
	//	vec2(-offset, -offset), // ����
	//	vec2(0.0f, -offset),	// ����
	//	vec2(offset, -offset)	// ����
	//	);

	//float kernel[9] = float[](
	//	-1, -1, -1,
	//	-1, 9, -1,
	//	-1, -1, -1
	//	);

	//vec3 sampleTex[9];
	//for (int i = 0; i < 9; i++)
	//{
	//	sampleTex[i] = vec3(texture(textureIndex, v_TexCoord.st + offsets[i]));
	//}
	//vec3 col = vec3(0.0);
	//for (int i = 0; i < 9; i++)
	//	col += sampleTex[i] * kernel[i];

	//FragColor = vec4(col, 1.0);

	// ģ��
	//const float offset = 1.0 / 300.0;
	//vec2 offsets[9] = vec2[](
	//	vec2(-offset, offset),	// ����
	//	vec2(0.0f, offset),		// ����
	//	vec2(offset, offset),	// ����
	//	vec2(-offset, 0.0f),	// ��
	//	vec2(0.0f, 0.0f),		// ��
	//	vec2(offset, 0.0f),		// ��
	//	vec2(-offset, -offset), // ����
	//	vec2(0.0f, -offset),	// ����
	//	vec2(offset, -offset)	// ����
	//	);

	//float kernel[9] = float[](
	//	1.0 / 16, 2.0 / 16, 1.0 / 16,
	//	2.0 / 16, 4.0 / 16, 2.0 / 16,
	//	1.0 / 16, 2.0 / 16, 1.0 / 16
	//	);

	//vec3 sampleTex[9];
	//for (int i = 0; i < 9; i++)
	//{
	//	sampleTex[i] = vec3(texture(textureIndex, v_TexCoord.st + offsets[i]));
	//}
	//vec3 col = vec3(0.0);
	//for (int i = 0; i < 9; i++)
	//	col += sampleTex[i] * kernel[i];

	//FragColor = vec4(col, 1.0);

	// ��Ե���
	const float offset = 1.0 / 300.0;
	vec2 offsets[9] = vec2[](
		vec2(-offset, offset),	// ����
		vec2(0.0f, offset),		// ����
		vec2(offset, offset),	// ����
		vec2(-offset, 0.0f),	// ��
		vec2(0.0f, 0.0f),		// ��
		vec2(offset, 0.0f),		// ��
		vec2(-offset, -offset), // ����
		vec2(0.0f, -offset),	// ����
		vec2(offset, -offset)	// ����
		);

	float kernel[9] = float[](
		1, 1, 1,
		1, -8, 1,
		1, 1, 1
		);

	vec3 sampleTex[9];
	for (int i = 0; i < 9; i++)
	{
		sampleTex[i] = vec3(texture(textureIndex, v_TexCoord.st + offsets[i]));
	}
	vec3 col = vec3(0.0);
	for (int i = 0; i < 9; i++)
		col += sampleTex[i] * kernel[i];

	FragColor = vec4(col, 1.0);
};


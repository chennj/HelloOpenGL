// 当我们以光的透视图进行场景渲染的时候，我们会用一个比较简单的着色器，
// 这个着色器除了把顶点变换到光空间以外，不会做得更多了，
// 这个顶点着色器将一个单独模型的一个顶点，使用lightSpaceMatrix变换到光空间中。
#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main()
{
    gl_Position = lightSpaceMatrix * model * vec4(position, 1.0f);
}
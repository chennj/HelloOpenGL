// 片段着色器 
// -- 一般位于顶点着色器之后
#version 330 core
out vec4 FragColor;

in vec3 fColor;

void main()
{
    FragColor = vec4(fColor, 1.0);   
}

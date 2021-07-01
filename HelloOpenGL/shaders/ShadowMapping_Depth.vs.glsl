// �������Թ��͸��ͼ���г�����Ⱦ��ʱ�����ǻ���һ���Ƚϼ򵥵���ɫ����
// �����ɫ�����˰Ѷ���任����ռ����⣬�������ø����ˣ�
// ���������ɫ����һ������ģ�͵�һ�����㣬ʹ��lightSpaceMatrix�任����ռ��С�
#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main()
{
    gl_Position = lightSpaceMatrix * model * vec4(position, 1.0f);
}
/**
*
* TOPIC: Writing a Basic Renderer in OpenGL
* -- ע��
*	 what a material is basically a shader plus a set of data
*	 or a shader plus all of its uniforms
*
* ע�� -- basic expand �� -- basic��˳��Ҫ�ߵ�
* glewInit() ������ glfwMakeContextCurrent(window) ����
*
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		std::cout << "error :" << glewGetErrorString(err) << std::endl;
	}

	//����ˢ�¼��ʱ��
	GLCall(glfwSwapInterval(1));

	std::cout << glfwGetVersionString() << std::endl;
	//�ӡ�{}�������������Ϊ�˷�ֹ���ֵ���glfwTerminate();
	//vbo��ibo������������û�б����ö�������ѭ�����µĳ����޷��˳�
	//���仰˵�ڵ���glfwTerminate();֮ǰ��Ҫ�ȵ���vbo��ibo������
	//����,��Ϊ�����������õ������������
	//while (glGetError() != GL_NO_ERROR);��Ĭ����������
	//��˵һ��ӦΪvbo��ibo��new �����ģ�������Ҫdelete�Ż�ʧЧ
	//Ҳ����˵��step-09������ʹ���ǲ��������������ġ�
	{
		float positions[] = {
			-0.5f, -0.5f,	//0
			0.5f, -0.5f,	//1
			0.5f,  0.5f,	//2

			-0.5f,  0.5f	//3
		};
		unsigned int indices[] = {
			0, 1, 2,		//��һ�������ε����������������index��
			2, 3, 0			//�ڶ��������ε��������������
		};

		//VAO
		VertexArray va;
		//VBO
		VertexBuffer vb(positions, 4 * 2 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);
		//IBO
		IndexBuffer ib(indices, 6);
		//Shader
		Shader shader("Basic.shader");
		shader.Bind();

		//ʹ��uniform
		shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

		//���-��Ϊ�����ж��������Ҫ��Ⱦ
		va.Unbind();
		vb.Unbind();
		ib.Unbind();
		shader.Unbind();

		Renderer renderer;

		float r = 0.0f;
		float increment = 0.05f;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();

			//���°�shader
			shader.Bind();
			shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

			renderer.Draw(va, ib, shader);

			if (r > 1.0f) {
				increment = -0.05f;
			}
			else if (r < 0.0f) {
				increment = 0.05f;
			}
			r += increment;

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}

	}
	glfwTerminate();
	return 0;
}
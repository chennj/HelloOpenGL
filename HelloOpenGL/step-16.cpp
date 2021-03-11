/**
*
* TOPIC: Maths in OpenGL
*
* -- OpenGL �õ�����ѧ���ַ��https://github.com/g-truc/glm
* -- projection matrix						����ͶӰ����
* -- MVP = model view projection (matrix)	ģ����ͼͶӰ������
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
#include "Texture.h"

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "step-16.h"

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
			-0.5f, -0.5f,/*���½Ƕ�������*/ 0.0f, 0.0f,	/*��Ӧ����������*/
			 0.5f, -0.5f,/*���½Ƕ�������*/ 1.0f, 0.0f,	/*��Ӧ����������*/
			 0.5f,  0.5f,/*���ϽǶ�������*/ 1.0f, 1.0f,	/*��Ӧ����������*/

			-0.5f,  0.5f,/*���ϽǶ�������*/ 0.0f, 1.0f		/*��Ӧ����������*/
		};
		unsigned int indices[] = {
			0, 1, 2,		//��һ�������ε����������������index��
			2, 3, 0			//�ڶ��������ε��������������
		};

		/**
		*
		* blending ��Ϸ�ʽ
		* ������ͣ��� /doc/blend-*.jpg
		*/
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		//VAO
		VertexArray va;
		//VBO
		VertexBuffer vb(positions, 4 * 4 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(2);	// ��������(x,y)
		layout.Push<float>(2);	// ��������(s,t)
		va.AddBuffer(vb, layout);
		//IBO
		IndexBuffer ib(indices, 6);

		/**
		* ʹ������ͶӰ��������ͼ��
		* -- ��������������һ�ֽ���������ӳ�䵽��άƽ���ϵķ������ڶ�άƽ���ϣ������Զ������ʵ���ϲ����С��
		*	 ����͸��ͶӰ�෴��͸��ͶӰ������ϰ���ڿ����ģ������������ʵ���������գ����ԽԶ������ʵ����ԽС����������������ά��Ⱦ�ķ�����
		*/
		glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);	// �ݺ�� = 3��4 Զ���� 1��1
		//ʹ����һ�����꿴��ͼ����ʲô�仯--���ͼ�λ���Сһ��
		//glm::mat4 proj = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, -1.0f, 1.0f);	

		//Shader
		Shader shader("Basic.shader");
		shader.Bind();

		//ʹ��uniform
		shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
		//ʹ������ͶӰ����
		shader.SetUniformMat4f("u_MVP", proj);
		
		//����
		Texture texture("../res/texture/texture-01.png");
		texture.Bind();
		shader.SetUniform1i("u_Texture", 0/*��Ϊ���ǵ��������0���ۣ���Texture::Bind()*/);

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
/**
*
* TOPIC: Textures in OpenGL
*
* ����������뵽����ʱ�����������뵽��������Ϸ�е�3d��������һ��ͼ������Ȼ��һ����3d����
* ��������Photoshop��paint֮��Ķ����д�����ĳ��ͼ�������ͼ��Ӧ�ó����е�ĳ�ֱ�����������ͼ�񣬱����Ͼ�������
* ������������������Ǵ�����˶�����Ŀ�����
* ���԰�һ�����������һ����������Ⱦ����ʱ����ʹ�õ�ͼ��
* ע��
* ����Ҫ˵���ǣ���Ļ�Ͼ��ε����½������꣨0,0�������Ͻ����������һ�ˣ���OpenGL��������˵���ǣ�1,1����
* ��Ϊ����ʵ���ϲ�û�н�����󶨵��ض��Ĵ�С����������Ҳû�н���������󶨵��ض��ķֱ��ʣ���Ϊ���������64X64
* Ҳ����ʹ1000X1000������������Ҫ���Ļ�������Ϊ���ǵľ��ε�ÿ������ָ����Ӧ����������ĸ�����
* Ȼ��Ƭ����ɫ�������������֮���ֵ����ˣ�������Ⱦһ�����أ���������������м�һ�㣬��Ӧ��Ƭ����ɫ����ѡ��
* ��������Ӧ���������������һ���λ�õ��������ꡣ
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
		//Shader
		Shader shader("Basic.shader");
		shader.Bind();

		//ʹ��uniform
		shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

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
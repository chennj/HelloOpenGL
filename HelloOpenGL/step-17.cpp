/**
*
* TOPIC: Projection Matrices in OpenGL
*
* -- OpenGL �õ�����ѧ���ַ��https://github.com/g-truc/glm
* -- orthod projection		����ͶӰһ������2d
* -- perspective projection ͸��ͶӰһ������3d
* -- normalized device coordinate (space)	
*	 ��׼���豸����(�ռ�)/�淶���豸����(�ռ�)
*	 �������Ҷ��� [-1,1]
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
		/**
		*
		* ����������positions[]��ֵ�Ŵ�8�������ͼ�ν�ֻ��ʾ�м��һ����,����ͼ��Ҳ�Ŵ���8��
		* -- Ϊʲô�أ�
		*	 ��Ϊ���潨��������ͶӰ ��glm::mat4 proj�� 
		*	 �����ӿڣ����ҿ����[-2,2] = 4 ���¸߶��� [-1.5,1.5] = 3 
		*	 �������Χ֮���ͼ�ζ�������ʾ
		*/
		float positions[] = {
			-0.5f, -0.5f,/*���½Ƕ�������*/ 0.0f, 0.0f,	/*��Ӧ����������*/
			 0.5f, -0.5f,/*���½Ƕ�������*/ 1.0f, 0.0f,	/*��Ӧ����������*/
			 0.5f,  0.5f,/*���ϽǶ�������*/ 1.0f, 1.0f,	/*��Ӧ����������*/

			-0.5f,  0.5f,/*���ϽǶ�������*/ 0.0f, 1.0f		/*��Ӧ����������*/
		};
		//float positions[] = {
		//	-4.0f, -4.0f,/*���½Ƕ�������*/ 0.0f, 0.0f,	/*��Ӧ����������*/
		//	 4.0f, -4.0f,/*���½Ƕ�������*/ 1.0f, 0.0f,	/*��Ӧ����������*/
		//	 4.0f,  4.0f,/*���ϽǶ�������*/ 1.0f, 1.0f,	/*��Ӧ����������*/

		//	-4.0f,  4.0f,/*���ϽǶ�������*/ 0.0f, 1.0f		/*��Ӧ����������*/
		//};

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
		* -- �����������ͶӰ���������
		*	 ���ڽ����εĶ���ת������׼���豸����(�ռ�)��Ҳ����[-1,1]
		*	 �������һ��-0.5�Ķ��㣬����������ߵ�һ��������ķ�֮һ����Ϊ0.5��2���ķ�֮һ��
		*	 ���ԣ���ô��������������λ��ת����-1��1֮�䣬��ô0��-1֮����ķ�֮һ��0.25
		*/
		glm::mat4 proj = glm::ortho(-0.5f, 3.5f, -0.5f, 2.5f, -1.0f, 1.0f);	// �ݺ�� = 3��4 Զ���� 1��1
		//glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);	// �ݺ�� = 3��4 Զ���� 1��1
		//ʹ����һ�����꿴��ͼ����ʲô�仯--���ͼ�λ���Сһ��
		//glm::mat4 proj = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, -1.0f, 1.0f);	
		/**
		* ��������ͶӰ���������
		* ��result_test���϶ϵ�ȥ����vp_test������������proj_test��֮��
		* result_test��(x,y)����˶���
		* �����(-1.0,-0.3] �����Ͻǵ����걻ӳ�䵽����Ļ�����²�,Ҳ������ʾͼ�����ڵ�λ�á�
		*/
		//proj_test�������ǽ�Ҫ���ݸ�shader������ͶӰ����proj
		glm::mat4 proj_test = glm::ortho(-0.5f, 3.5f, -0.5f, 2.5f, -1.0f, 1.0f);
		//vp_test��positions[]���ϽǶ�������
		glm::vec4 vp_test(-0.5f, 0.5f, 0.0f, 1.0f);
		//����ͶӰ����proj_test��Ҫ�Ѷ�������vp_testת����[-1,1]�ı�׼���豸����ռ�
		//���Ľ������Basic.shader�е�gl_Position = u_MVP * position��ֻ����������һ������ĵ�ת�����
		//����������ϽǵĶ����ƶ�������Ļ�����²���Ҳ��������ͼ����ʽ�����ӿڵ����½ǡ�
		glm::vec4 result_test = proj_test * vp_test;

		//shader																																		
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
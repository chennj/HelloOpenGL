/**
*
* TOPIC: ImGui in OpenGL
*
* -- OpenGL �õ�����ѧ���ַ��https://github.com/g-truc/glm
* -- ImGui ���ڿ�ܵ�ַ��https://github.com/ocornut/imgui
* -- orthod projection		����ͶӰһ������2d
* -- perspective projection ͸��ͶӰһ������3d
* -- normalized device coordinate (space)
*	 ��׼���豸����(�ռ�)/�淶���豸����(�ռ�)
*	 �������Ҷ��� [-1,1]
* -- MVP = model view projection (matrix)	ģ����ͼͶӰ������
*	 ʵ�����������������ֱ任
*	 ģ�ͱ任(����) -> ��ͼ�任(����) -> ͶӰ�任(����)
*	 ģ�ͱ任�����۲������������ת�����š����С�ƽ�Ʊ任
*	 ��ͼ�任���任�۲��ߵ�λ�ã��ڳ����з��������������ָ��ĳ������
*	 ͶӰ�任��ͶӰ�任����ģ�ͱ任->��ͼ�任֮��Ӧ�õ������ϣ�����ָ��һ����ɵĳ���
��������ͼģ�ͱ任������ɣ������ͶӰ����Ļ�ϵ�����ͼ��
��������Ķ�������ת��������׼���豸����ռ䣬��-1��1֮�䡣
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

#include "imgui\imgui.h"
#include "imgui\imgui_impl_glfw_gl3.h"

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
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
		* �����ݸ�Basic.shader��gl_Position
		*/
		float positions[] = {
			100.0f,	100.0f,	/*���½Ƕ�������*/	0.0f, 0.0f,		/*��Ӧ����������*/
			200.0f,	100.0f,	/*���½Ƕ�������*/	1.0f, 0.0f,		/*��Ӧ����������*/
			200.0f,	200.0f,	/*���ϽǶ�������*/	1.0f, 1.0f,		/*��Ӧ����������*/

			100.0f,	200.0f,	/*���ϽǶ�������*/	0.0f, 1.0f		/*��Ӧ����������*/
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
		* -- �����������ͶӰ���������
		*	 ���ڽ����εĶ���ת������׼���豸����(�ռ�)��Ҳ����[-1,1]
		*	 �������һ��-0.5�Ķ��㣬����������ߵ�һ��������ķ�֮һ����Ϊ0.5��2���ķ�֮һ��
		*	 ���ԣ���ô��������������λ��ת����-1��1֮�䣬��ô0��-1֮����ķ�֮һ��0.25
		*/
		glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);	// �ݺ�� = 9��16 ���ֺʹ�����ͬ���ݺ�� Զ���� 1��1
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));

		//shader																																		
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

		ImGui::CreateContext();
		ImGui_ImplGlfwGL3_Init(window, true);
		ImGui::StyleColorsDark();

		glm::vec3 translation(200, 200, 0);

		float r = 0.0f;
		float increment = 0.05f;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();

			ImGui_ImplGlfwGL3_NewFrame();

			glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
			glm::mat4 mvp = proj * model * view;

			//���°�shader
			shader.Bind();
			shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
			shader.SetUniformMat4f("u_MVP", mvp);

			renderer.Draw(va, ib, shader);

			if (r > 1.0f) {
				increment = -0.05f;
			}
			else if (r < 0.0f) {
				increment = 0.05f;
			}
			r += increment;

			{
				ImGui::SliderFloat3("Translation", &translation.x, 0.0f, 960.0f);
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			}

			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}

	}

	// Cleanup
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}
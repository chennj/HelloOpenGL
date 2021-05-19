/**
*
* TOPIC: Shader Abstraction in OpenGL
*
*
*
*
*
* 注意 -- basic expand 和 -- basic的顺序不要颠倒
* glewInit() 必须在 glfwMakeContextCurrent(window) 后定义
*
*/

#ifdef __RUN__
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
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

	//缓冲刷新间隔时间
	GLCall(glfwSwapInterval(1));

	std::cout << glfwGetVersionString() << std::endl;
	//加”{}“这个东西，是为了防止出现调用glfwTerminate();
	//vbo或ibo的析构函数还没有被调用而出现死循环导致的程序无法退出
	//换句话说在调用glfwTerminate();之前需要先调用vbo和ibo的析构
	//函数,因为这两个对象都用到了这个东西：
	//while (glGetError() != GL_NO_ERROR);幽默不！！！！
	//再说一下应为vbo和ibo是new 出来的，所以需要delete才会失效
	//也就是说像step-09中那样使用是不会出现这种情况的。
	{
		float positions[] = {
			-0.5f, -0.5f,	//0
			0.5f, -0.5f,	//1
			0.5f,  0.5f,	//2

			-0.5f,  0.5f	//3
		};
		unsigned int indices[] = {
			0, 1, 2,		//第一个三角形的三个顶点的索引（index）
			2, 3, 0			//第二个三角形的三个顶点的索引
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

		//使用uniform
		shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

		//解绑-因为可能有多个物体需要渲染
		va.Unbind();
		vb.Unbind();
		ib.Unbind();
		shader.Unbind();

		float r = 0.0f;
		float increment = 0.05f;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT);

			//重新绑定shader
			shader.Bind();
			shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

			va.Bind();
			ib.Bind();

			GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

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
#endif
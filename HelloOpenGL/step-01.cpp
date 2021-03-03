/**
*
* TOPIC: Vertex Buffers and Drawing a Triangle
*
* 注意 -- basic expand 和 -- basic的顺序不要颠倒
* glewInit() 必须在 glfwMakeContextCurrent(window) 后定义
* 
*/
#ifdef _RUN_
#include <iostream>
// import opengl third part library
// ---------------------------------
// -- basic expand
// -- 使用静态链接库 glew32s.lib,需要定义 GLEW_STATIC
#define GLEW_STATIC
#include <GLEW/glew.h>
#pragma comment(lib,"glew32s.lib")
// -- basic
#include <GLFW/glfw3.h>
#pragma comment(lib,"glfw3.lib")
// ---------------------------------
#pragma comment(lib,"OpenGL32.Lib")

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

	std::cout << glfwGetVersionString() << std::endl;

	float positions[6] = {
		-0.5f, -0.5f,
		 0.0f,  0.5f,
		 0.5f, -0.5f
	};

	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

#endif
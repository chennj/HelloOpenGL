/**
*
* TOPIC: How Shaders Work in OpenGL
* -- Vertex Shader and Fragment Shader also called Pixel Shader
* -- ���û滭�����ǣ�һ���������һ�� vertex shader��һ�����ص���һ��Fragment Shader
* -- Primary purpose of a Vextex Shader is to tell OpenGL where you want that vertex to be 
*	 in your screen space.
* -- Primary purpose of a Fragment Shader is to actually decide which color that pixel is 
*	 supposed to be.
* -- ���� Vertex Shader ���㲢��ɫ�����ζ��㣬Ȼ�����Ź��ߣ�pipeline�������ݴ��ݸ� Fragment Shader��
*    ����ȡ���㲢��ɫ����֮������ء�
*
* ע�� -- basic expand �� -- basic��˳��Ҫ�ߵ�
* glewInit() ������ glfwMakeContextCurrent(window) ����
*
*/

#ifdef __RUN__
#include <iostream>
// import opengl third part library
// ---------------------------------
// -- basic expand
// -- ʹ�þ�̬���ӿ� glew32s.lib,��Ҫ���� GLEW_STATIC
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

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

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

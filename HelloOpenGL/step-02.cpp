/**
*
* TOPIC: Vertex Attributes and Layouts
* -- glVertexAttribPointer
	 (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer) 最后两个参数说明
* -- stride : 
*	 stride is as the documentation states, the byte offset between 
*	 consecutive generic vertex attributes, this is worded, in not sure why the why they worded it like this, 
*    but basically what the stride is the amount of bytes between each vertex, so again in our example, 
*	 if we have a position, we have a texture coordinate and we have a normal, 
*    let's just say the position is a component vector, then the texture coordinate is 
*    a two component vector two floats and then the normal is three components once again, right so, 
*	 we have three floats that's 12 bytes plus two floats for the texture coordinate 
*	 that's another eight bytes or on 20 bytes and then another 12 bytes for the normal, 
*	 so 32 bytes, that is our stride. because it's basically this site the size of each vertex, 
*	 the reason this is important is because if opengl and the reason it's called stride as well is 
*	 because we if we want to jump from one vertex to the next so for example i want to look up index 
*	 number one so from zero, zero is our first vertex i want to jump to our second vertex which is index number one, 
*	 i need to go plus 32 bytes into the buffer, so we have a pointer to the beginning of our buffer 
*	 and i go 32 bytes into it, i should be at the very start of the next start of the next vertex that is
*	 what the stride is, so just, remember that.
* -- pointer :
*	 pointer is a pointer into the actual attribute, so, this ia already inside the space of the vertex, 
*	 right, so you're just pretend don't worry about how many vertices you have anymore,
*	 pretend you only have one vertex, i have one vertex and inside there i have position,
*	 texture corner and normal, what is the offset of each of those in bytes, right,
*	 so for position the offset will be zero because the very frist byte of our buffer,
*	 the very first byte of our actual vertex, is the position, right,
*	 then we go 12 bytes forward and we reach the beginning of the texture coordinate,
*	 so for my texture quarter attribute, this value would be 12,
*	 because 12 bytes into it, is where we begin and finally 8 bytes into
*	 that we get to the vertex normal, right, so 20 bytes from the beginning is my vertex normal format,
*	 so for my vertex normal attribute 20 would be the value of this pointer,
*	 now instead of just writing generic things like 0 12 and 20,
*	 because that can get a bit confusing and not only that,
*	 but if you actual rearrange you layout or something like that
*	 you'll have to reach out recalculating or manually, there are macros and kind of
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
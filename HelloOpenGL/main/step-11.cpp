/**
*
* TOPIC: Buffer Layout Abstraction in OpenGL
*
* Abstracting OpenGL into Classes
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

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
{
	std::fstream stream(filepath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				//set mode to vertex
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				//set mode to fragment
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			ss[(int)type] << line << "\n";
		}
	}

	return{ ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	//建立一个着色器
	GLCall(unsigned int id = glCreateShader(type));
	//需要编译的源码字符串
	const char* src = source.c_str();
	//将源码和着色器关联起来
	GLCall(glShaderSource(id, 1, &src, nullptr));
	//编译源码
	GLCall(glCompileShader(id));

	// TODO: Synatx errors handling
	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	GLCall(unsigned int program = glCreateProgram());
	//建立顶点着色器
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	//建立片段着色器
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	//将这两个着色器都附加到我们的程序中，在程序中指定要附加的着色器
	//把它想象成编译c++代码，我们有两个不同的文件: vertexShader 和 fragmentShader，
	//将他们放入我们的 propram，然后就可以使用他们。
	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	//将着色器链接进program后，立即释放这两个着色器
	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}

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

		std::string filepath = "Basic.shader";
		ShaderProgramSource source = ParseShader(filepath);

		unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
		GLCall(glUseProgram(shader));

		//使用uniform
		GLCall(int location = glGetUniformLocation(shader, "u_Color"));
		ASSERT(location != -1);
		GLCall(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f));

		//解绑-因为可能有多个物体需要渲染
		GLCall(glBindVertexArray(0));
		GLCall(glUseProgram(0));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

		float r = 0.0f;
		float increment = 0.05f;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT);

			//重新绑定shader
			GLCall(glUseProgram(shader));
			GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));

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

		GLCall(glDeleteProgram(shader));
	}
	glfwTerminate();
	return 0;
}
#endif
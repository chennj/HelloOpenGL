/**
*
* TOPIC: Uniforms in OpenGL
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

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << ")"
			<< function << " "
			<< file << ":" << line
			<< std::endl;
		return false;
	}
	return true;
}

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

	//绑定顶点缓冲区
	unsigned int buffer;
	GLCall(glGenBuffers(1, &buffer));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
	GLCall(glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW));
	//描述绑定的顶点缓冲区的布局
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));

	unsigned int ibo;	//index buffer object = ibo;
	GLCall(glGenBuffers(1, &ibo));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));
	//------------------------

	std::string filepath = "Basic.shader";
	ShaderProgramSource source = ParseShader(filepath);

	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
	GLCall(glUseProgram(shader));

	//使用uniform
	GLCall(int location = glGetUniformLocation(shader, "u_Color"));
	ASSERT(location != -1);
	float r = 0.0f;
	float increment = 0.05f;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));
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

	glfwTerminate();
	return 0;
}
#endif
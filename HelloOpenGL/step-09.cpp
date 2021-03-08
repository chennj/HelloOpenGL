/**
*
* TOPIC: Vertex Arrays in OpenGL
*
* Vertex Buffer（Object) 与 Vertex Arrays (Object)的异同
* -- 它们基本上是一种将顶点缓冲区绑定到该顶点缓冲区布局的特定规范的方法
* -- -------------------------------------------------------------------------------------------
* -- 首先必须明确一点：如果你在程序中没有手动建立一个VAO（Vertex Buffer Object)，并不代表它不存在，
*	 只不过是系统帮你建了一个而已
*	 简单的理解：绑定Vertex Arrays = 绑定Vertex Buffer + 绑定与这个Vertex Buffer相关联的属性（如顶点布局)
*    关于 VAO的使用建议：
*	 为你创建的每一块几何体，创建一个顶点数组对象(VAO)与其对应，这样在绘图是，只需要一个指针指定不同的VAO就可以了，
*	 而不需要重新绑定一大堆的东西
* -- -------------------------------------------------------------------------------------------
* -- 顶点数组对象实际上让我们做的是绑定顶点规范，
*	 我们通过使用搜索顶点指定一个trip指针到一个实际的顶点缓冲区或一系列的顶点缓冲区，这取决于我们实际如何组织它
* -- 顶点数组对象(Vertex Array Object(VAO))将包含一个或多个顶点缓冲区(Vertex Buffer Object(VBO))
*	 与实际类型的顶点规范或顶点布局之间的绑定，因此换句话说，顶点属性指针实际上将绑定一个实际的顶点缓冲区和数组缓冲区
* -- 技术上来说，VAOs是必须的。它们实际上正在使用中，即使没有显式的定义他们。如果你没有显式的建立他们，OpenGL为了
*	 兼容，也会缺省的为你建立一个。如果指定使用核心特征（GLFW_OPENGL_CORE_PROFILE），就必须显式的建立一个VAO
*
* 注意 -- basic expand 和 -- basic的顺序不要颠倒
* glewInit() 必须在 glfwMakeContextCurrent(window) 后定义
*
*/

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

	//演示：在这里如果指定使用核心模式（GLFW_OPENGL_CORE_PROFILE），就必须显式的建立一个VAO （index buffer object)
	//否则狗屎，当然如果指定使用兼容模式（GLFW_OPENGL_COMPAT_PROFILE），这一切正常，不过这时脱裤子放屁多此一举，OpenGL缺省
	//就已经是兼容模式。
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

	//VAO绑定过程说明
	//-----------------------------------------------------
	//在1.处实际上没有将任何缓冲区绑定到VAO。
	//在3.处才真正将2.处的缓冲区（buffer）绑定到1.处的vao上，其中3.
	//处的glVertexAttribPointer的第一个参数0，指定绑定到vao的位置
	//如果需要改变绑定的位置只需要改变该参数就行。
	//-----------------------------------------------------
	//使用核心模式（GLFW_OPENGL_CORE_PROFILE），则必须手动建立VAO
	unsigned int vao;
	GLCall(glGenVertexArrays(1, &vao));
	GLCall(glBindVertexArray(vao));			// 1.
	
	//绑定顶点缓冲区
	unsigned int buffer;
	GLCall(glGenBuffers(1, &buffer));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));	// 2.
	GLCall(glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW));
	//描述绑定的顶点缓冲区的布局
	GLCall(glEnableVertexAttribArray(0));	//指定VAO的第一个位置有效
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));	// 3.
	//----------------------------------------------------

	//IBO
	//----------------------------------------------------
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
		//如果使用了VAO，则下面的属性不用再绑定
		//重新绑定vertex buffer
		//GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
		//GLCall(glEnableVertexAttribArray(0));
		//GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));
		//只需绑定VAO
		GLCall(glBindVertexArray(vao));
		//重新绑定index buffer
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));

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
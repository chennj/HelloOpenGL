/**
*
* TOPIC: Writting a Shader in OpenGL
*
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

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	//����һ����ɫ��
	unsigned int id = glCreateShader(type);
	//��Ҫ�����Դ���ַ���
	const char* src = source.c_str();
	//��Դ�����ɫ����������
	glShaderSource(id, 1, &src, nullptr);
	//����Դ��
	glCompileShader(id);

	// TODO: Synatx errors handling
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile "<< (type == GL_VERTEX_SHADER ? "vertex" : "fragment") <<" shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	//����������ɫ��
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	//����Ƭ����ɫ��
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	//����������ɫ�������ӵ����ǵĳ����У��ڳ�����ָ��Ҫ���ӵ���ɫ��
	//��������ɱ���c++���룬������������ͬ���ļ�: vertexShader �� fragmentShader��
	//�����Ƿ������ǵ� propram��Ȼ��Ϳ���ʹ�����ǡ�
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	//����ɫ�����ӽ�program�������ͷ���������ɫ��
	glDeleteShader(vs);
	glDeleteShader(fs);

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

	std::string vertexShaderSourceCode =
		"#version 330 core\n"				//it means that it's going to let us use any kind of deprected functions 
											//or any time that would result in a shader compilation because  we just  
											//want to make sure that we're using the latest and the greatest 
											//that we have known
		"\n"
		"layout(location = 0) in vec4 position;"	// �������涨���positions����ĵ�ַ����һ����ά����
													// position = o index of above 'float positions[6]'
		"\n"
		"void main()\n"						//it is main function for our shader 
		"{\n"
		"	gl_Position = position;\n"
		"}\n";

	std::string fragmentShaderSourceCode =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) out vec4 color;"		// �����ɫ��������һ����ά����	
		"\n"
		"void main()\n"
		"{\n"
		"	color = vec4(1.0, 0.0, 0.0, 1.0);\n"
		"}\n";

	unsigned int shader = CreateShader(vertexShaderSourceCode, fragmentShaderSourceCode);
	glUseProgram(shader);

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

	glDeleteShader(shader);

	glfwTerminate();
	return 0;
}
#endif
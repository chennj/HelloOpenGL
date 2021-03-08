/**
*
* TOPIC: Vertex Arrays in OpenGL
*
* Vertex Buffer��Object) �� Vertex Arrays (Object)����ͬ
* -- ���ǻ�������һ�ֽ����㻺�����󶨵��ö��㻺�������ֵ��ض��淶�ķ���
* -- -------------------------------------------------------------------------------------------
* -- ���ȱ�����ȷһ�㣺������ڳ�����û���ֶ�����һ��VAO��Vertex Buffer Object)�����������������ڣ�
*	 ֻ������ϵͳ���㽨��һ������
*	 �򵥵���⣺��Vertex Arrays = ��Vertex Buffer + �������Vertex Buffer����������ԣ��綥�㲼��)
*    ���� VAO��ʹ�ý��飺
*	 Ϊ�㴴����ÿһ�鼸���壬����һ�������������(VAO)�����Ӧ�������ڻ�ͼ�ǣ�ֻ��Ҫһ��ָ��ָ����ͬ��VAO�Ϳ����ˣ�
*	 ������Ҫ���°�һ��ѵĶ���
* -- -------------------------------------------------------------------------------------------
* -- �����������ʵ���������������ǰ󶨶���淶��
*	 ����ͨ��ʹ����������ָ��һ��tripָ�뵽һ��ʵ�ʵĶ��㻺������һϵ�еĶ��㻺��������ȡ��������ʵ�������֯��
* -- �����������(Vertex Array Object(VAO))������һ���������㻺����(Vertex Buffer Object(VBO))
*	 ��ʵ�����͵Ķ���淶�򶥵㲼��֮��İ󶨣���˻��仰˵����������ָ��ʵ���Ͻ���һ��ʵ�ʵĶ��㻺���������黺����
* -- ��������˵��VAOs�Ǳ���ġ�����ʵ��������ʹ���У���ʹû����ʽ�Ķ������ǡ������û����ʽ�Ľ������ǣ�OpenGLΪ��
*	 ���ݣ�Ҳ��ȱʡ��Ϊ�㽨��һ�������ָ��ʹ�ú���������GLFW_OPENGL_CORE_PROFILE�����ͱ�����ʽ�Ľ���һ��VAO
*
* ע�� -- basic expand �� -- basic��˳��Ҫ�ߵ�
* glewInit() ������ glfwMakeContextCurrent(window) ����
*
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

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
	//����һ����ɫ��
	GLCall(unsigned int id = glCreateShader(type));
	//��Ҫ�����Դ���ַ���
	const char* src = source.c_str();
	//��Դ�����ɫ����������
	GLCall(glShaderSource(id, 1, &src, nullptr));
	//����Դ��
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
	//����������ɫ��
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	//����Ƭ����ɫ��
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	//����������ɫ�������ӵ����ǵĳ����У��ڳ�����ָ��Ҫ���ӵ���ɫ��
	//��������ɱ���c++���룬������������ͬ���ļ�: vertexShader �� fragmentShader��
	//�����Ƿ������ǵ� propram��Ȼ��Ϳ���ʹ�����ǡ�
	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	//����ɫ�����ӽ�program�������ͷ���������ɫ��
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

	//��ʾ�����������ָ��ʹ�ú���ģʽ��GLFW_OPENGL_CORE_PROFILE�����ͱ�����ʽ�Ľ���һ��VAO ��index buffer object)
	//����ʺ����Ȼ���ָ��ʹ�ü���ģʽ��GLFW_OPENGL_COMPAT_PROFILE������һ��������������ʱ�ѿ��ӷ�ƨ���һ�٣�OpenGLȱʡ
	//���Ѿ��Ǽ���ģʽ��
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

	//����ˢ�¼��ʱ��
	GLCall(glfwSwapInterval(1));

	std::cout << glfwGetVersionString() << std::endl;

	float positions[] = {
		-0.5f, -0.5f,	//0
		0.5f, -0.5f,	//1
		0.5f,  0.5f,	//2

		-0.5f,  0.5f	//3
	};
	unsigned int indices[] = {
		0, 1, 2,		//��һ�������ε����������������index��
		2, 3, 0			//�ڶ��������ε��������������
	};

	//VAO�󶨹���˵��
	//-----------------------------------------------------
	//��1.��ʵ����û�н��κλ������󶨵�VAO��
	//��3.����������2.���Ļ�������buffer���󶨵�1.����vao�ϣ�����3.
	//����glVertexAttribPointer�ĵ�һ������0��ָ���󶨵�vao��λ��
	//�����Ҫ�ı�󶨵�λ��ֻ��Ҫ�ı�ò������С�
	//-----------------------------------------------------
	//ʹ�ú���ģʽ��GLFW_OPENGL_CORE_PROFILE����������ֶ�����VAO
	unsigned int vao;
	GLCall(glGenVertexArrays(1, &vao));
	GLCall(glBindVertexArray(vao));			// 1.
	
	//�󶨶��㻺����
	unsigned int buffer;
	GLCall(glGenBuffers(1, &buffer));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));	// 2.
	GLCall(glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW));
	//�����󶨵Ķ��㻺�����Ĳ���
	GLCall(glEnableVertexAttribArray(0));	//ָ��VAO�ĵ�һ��λ����Ч
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

	//ʹ��uniform
	GLCall(int location = glGetUniformLocation(shader, "u_Color"));
	ASSERT(location != -1);
	GLCall(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f));


	//���-��Ϊ�����ж��������Ҫ��Ⱦ
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

		//���°�shader
		GLCall(glUseProgram(shader));
		GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));
		//���ʹ����VAO������������Բ����ٰ�
		//���°�vertex buffer
		//GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
		//GLCall(glEnableVertexAttribArray(0));
		//GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));
		//ֻ���VAO
		GLCall(glBindVertexArray(vao));
		//���°�index buffer
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
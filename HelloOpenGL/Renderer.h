#ifndef __RENDERER_H_
#define __RENDERER_H_

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

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

class Renderer
{
private:

public:
	void Clear() const;
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};
#endif
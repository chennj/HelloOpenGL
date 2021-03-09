/**
*
* TOPIC: Textures in OpenGL
*
* 当大多数人想到纹理时，他们真正想到的是在游戏中的3d对象上有一个图像，它当然不一定是3d对象，
* 而是你在Photoshop或paint之类的东西中创建的某种图像，在你的图形应用程序中的某种表面上有这种图像，本质上就是纹理，
* 或者至少是纹理，这就是大多数人对纹理的看法。
* 可以把一个纹理想象成一个我们在渲染东西时可以使用的图像
* 注：
* 我们要说的是，屏幕上矩形的左下角是坐标（0,0），右上角是纹理的另一端，用OpenGL的术语来说就是（1,1）。
* 因为纹理实际上并没有将纹理绑定到特定的大小，或者我们也没有将纹理坐标绑定到特定的分辨率，因为纹理可以是64X64
* 也可能使1000X1000。所以我们需要做的基本上是为我们的矩形的每个顶点指定它应该是纹理的哪个区域，
* 然后片段着色器将在这个区域之间插值。因此，我们渲染一个像素，比如两个顶点的中间一点，相应的片段着色器会选择
* 纹理上相应的两个顶点坐标的一半的位置的纹理坐标。
*
* 注意 -- basic expand 和 -- basic的顺序不要颠倒
* glewInit() 必须在 glfwMakeContextCurrent(window) 后定义
*
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

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
			-0.5f, -0.5f,/*左下角顶点坐标*/ 0.0f, 0.0f,	/*对应的纹理坐标*/
			 0.5f, -0.5f,/*右下角顶点坐标*/ 1.0f, 0.0f,	/*对应的纹理坐标*/
			 0.5f,  0.5f,/*右上角顶点坐标*/ 1.0f, 1.0f,	/*对应的纹理坐标*/

			-0.5f,  0.5f,/*左上角顶点坐标*/ 0.0f, 1.0f		/*对应的纹理坐标*/
		};
		unsigned int indices[] = {
			0, 1, 2,		//第一个三角形的三个顶点的索引（index）
			2, 3, 0			//第二个三角形的三个顶点的索引
		};

		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		//VAO
		VertexArray va;
		//VBO
		VertexBuffer vb(positions, 4 * 4 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(2);	// 顶点坐标(x,y)
		layout.Push<float>(2);	// 纹理坐标(s,t)
		va.AddBuffer(vb, layout);
		//IBO
		IndexBuffer ib(indices, 6);
		//Shader
		Shader shader("Basic.shader");
		shader.Bind();

		//使用uniform
		shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

		//纹理
		Texture texture("../res/texture/texture-01.png");
		texture.Bind();
		shader.SetUniform1i("u_Texture", 0/*因为我们的纹理绑定在0卡槽，看Texture::Bind()*/);

		//解绑-因为可能有多个物体需要渲染
		va.Unbind();
		vb.Unbind();
		ib.Unbind();
		shader.Unbind();

		Renderer renderer;

		float r = 0.0f;
		float increment = 0.05f;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();

			//重新绑定shader
			shader.Bind();
			shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

			renderer.Draw(va, ib, shader);

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
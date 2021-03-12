/**
*
* TOPIC: Projection Matrices in OpenGL
*
* -- OpenGL 用到的数学库地址：https://github.com/g-truc/glm
* -- orthod projection		正交投影一般用于2d
* -- perspective projection 透视投影一般用于3d
* -- normalized device coordinate (space)	
*	 标准化设备坐标(空间)/规范化设备坐标(空间)
*	 上下左右都是 [-1,1]
* -- MVP = model view projection (matrix)	模型视图投影（矩阵）
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

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "step-16.h"

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
		/**
		*
		* 如果将下面的positions[]的值放大8倍，结果图形将只显示中间的一部分,并且图形也放大了8倍
		* -- 为什么呢？
		*	 因为下面建立的正交投影 “glm::mat4 proj” 
		*	 它的视口：左右宽度是[-2,2] = 4 上下高度是 [-1.5,1.5] = 3 
		*	 在这个范围之外的图形都将不显示
		*/
		float positions[] = {
			-0.5f, -0.5f,/*左下角顶点坐标*/ 0.0f, 0.0f,	/*对应的纹理坐标*/
			 0.5f, -0.5f,/*右下角顶点坐标*/ 1.0f, 0.0f,	/*对应的纹理坐标*/
			 0.5f,  0.5f,/*右上角顶点坐标*/ 1.0f, 1.0f,	/*对应的纹理坐标*/

			-0.5f,  0.5f,/*左上角顶点坐标*/ 0.0f, 1.0f		/*对应的纹理坐标*/
		};
		//float positions[] = {
		//	-4.0f, -4.0f,/*左下角顶点坐标*/ 0.0f, 0.0f,	/*对应的纹理坐标*/
		//	 4.0f, -4.0f,/*右下角顶点坐标*/ 1.0f, 0.0f,	/*对应的纹理坐标*/
		//	 4.0f,  4.0f,/*右上角顶点坐标*/ 1.0f, 1.0f,	/*对应的纹理坐标*/

		//	-4.0f,  4.0f,/*左上角顶点坐标*/ 0.0f, 1.0f		/*对应的纹理坐标*/
		//};

		unsigned int indices[] = {
			0, 1, 2,		//第一个三角形的三个顶点的索引（index）
			2, 3, 0			//第二个三角形的三个顶点的索引
		};

		/**
		*
		* blending 混合方式
		* 具体解释：看 /doc/blend-*.jpg
		*/
		GLCall(glEnable(GL_BLEND));
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

		/**
		* 使用正交投影矩阵修正图形
		* -- 正交矩阵本质上是一种将所有坐标映射到二维平面上的方法，在二维平面上，距离较远的物体实际上不会变小。
		*	 这与透视投影相反，透视投影是我们习惯于看到的，如果我们在现实生活中拍照，离得越远的物体实际上越小，这是我们用于三维渲染的方法。
		* -- 下面这个正交投影矩阵的作用
		*	 用于将矩形的顶点转换到标准化设备坐标(空间)，也就是[-1,1]
		*	 如果我有一个-0.5的顶点，它是向最左边的一个方向的四分之一，因为0.5是2的四分之一，
		*	 所以，那么这个正交矩阵将这个位置转换成-1和1之间，那么0到-1之间的四分之一是0.25
		*/
		glm::mat4 proj = glm::ortho(-0.5f, 3.5f, -0.5f, 2.5f, -1.0f, 1.0f);	// 纵横比 = 3：4 远近比 1：1
		//glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);	// 纵横比 = 3：4 远近比 1：1
		//使用这一组坐标看看图形有什么变化--结果图形会缩小一倍
		//glm::mat4 proj = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, -1.0f, 1.0f);	
		/**
		* 测试正交投影矩阵的作用
		* 在result_test加上断点去看看vp_test经过正交矩阵proj_test的之后
		* result_test的(x,y)变成了多少
		* 结果是(-1.0,-0.3] 即左上角的坐标被映射到了屏幕的左下部,也就是演示图形所在的位置。
		*/
		//proj_test就是我们将要传递给shader的正交投影矩阵proj
		glm::mat4 proj_test = glm::ortho(-0.5f, 3.5f, -0.5f, 2.5f, -1.0f, 1.0f);
		//vp_test是positions[]左上角顶点坐标
		glm::vec4 vp_test(-0.5f, 0.5f, 0.0f, 1.0f);
		//正交投影矩阵proj_test将要把顶点坐标vp_test转换到[-1,1]的标准化设备坐标空间
		//它的结果就是Basic.shader中的gl_Position = u_MVP * position，只不过是其中一个顶点的的转换结果
		//结果就是左上角的顶点移动到了屏幕的左下部，也就是整个图形显式在了视口的左下角。
		glm::vec4 result_test = proj_test * vp_test;

		//shader																																		
		Shader shader("Basic.shader");
		shader.Bind();

		//使用uniform
		shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
		//使用正交投影矩阵
		shader.SetUniformMat4f("u_MVP", proj);

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
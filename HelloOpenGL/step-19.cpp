/**
*
* TOPIC: ImGui in OpenGL
*
* -- OpenGL 用到的数学库地址：https://github.com/g-truc/glm
* -- ImGui 窗口框架地址：https://github.com/ocornut/imgui
* -- orthod projection		正交投影一般用于2d
* -- perspective projection 透视投影一般用于3d
* -- normalized device coordinate (space)
*	 标准化设备坐标(空间)/规范化设备坐标(空间)
*	 上下左右都是 [-1,1]
* -- MVP = model view projection (matrix)	模型视图投影（矩阵）
*	 实际上是三个矩阵，三种变换
*	 模型变换(矩阵) -> 视图变换(矩阵) -> 投影变换(矩阵)
*	 模型变换：被观察物体自身的旋转、缩放、错切、平移变换
*	 视图变换：变换观察者的位置，在场景中放置照相机并让它指向某个方向
*	 投影变换：投影变换将在模型变换->视图变换之后应用到顶点上，它将指定一个完成的场景
（所有视图模型变换都已完成）是如何投影到屏幕上的最终图像。
即将物体的顶点坐标转换进到标准化设备坐标空间，即-1到1之间。
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

#include "imgui\imgui.h"
#include "imgui\imgui_impl_glfw_gl3.h"

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
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
		* 将传递给Basic.shader的gl_Position
		*/
		float positions[] = {
			100.0f,	100.0f,	/*左下角顶点坐标*/	0.0f, 0.0f,		/*对应的纹理坐标*/
			200.0f,	100.0f,	/*右下角顶点坐标*/	1.0f, 0.0f,		/*对应的纹理坐标*/
			200.0f,	200.0f,	/*右上角顶点坐标*/	1.0f, 1.0f,		/*对应的纹理坐标*/

			100.0f,	200.0f,	/*左上角顶点坐标*/	0.0f, 1.0f		/*对应的纹理坐标*/
		};

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
		glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);	// 纵横比 = 9：16 保持和窗口相同的纵横比 远近比 1：1
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));

		//shader																																		
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

		ImGui::CreateContext();
		ImGui_ImplGlfwGL3_Init(window, true);
		ImGui::StyleColorsDark();

		glm::vec3 translation(200, 200, 0);

		float r = 0.0f;
		float increment = 0.05f;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();

			ImGui_ImplGlfwGL3_NewFrame();

			glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
			glm::mat4 mvp = proj * model * view;

			//重新绑定shader
			shader.Bind();
			shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
			shader.SetUniformMat4f("u_MVP", mvp);

			renderer.Draw(va, ib, shader);

			if (r > 1.0f) {
				increment = -0.05f;
			}
			else if (r < 0.0f) {
				increment = 0.05f;
			}
			r += increment;

			{
				ImGui::SliderFloat3("Translation", &translation.x, 0.0f, 960.0f);
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			}

			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}

	}

	// Cleanup
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}
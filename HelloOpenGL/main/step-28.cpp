/**
*
* TOPIC: Writing a BATCH RENDERER
*
* -- OpenGL 用到的数学库地址：https://github.com/g-truc/glm
* -- ImGui 窗口框架地址：https://github.com/ocornut/imgui
* -- OpenGL API Document http://docs.gl/
*
* -- Batch Rendering : /doc/Batch Rendering.JPG
* -- 这时这个课程的最后一节了，代码做了很多修改，所以和原版有许多差别。
*	 个人感觉更适合学习，谢谢美国佬-佐夫。
*	 下面开始游戏引擎
*	 再长的路都会走完，除非停下或死去。完结撒花^-^.
*/
#ifdef __RUN__
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

#include "tests\TestClearColor.h"
#include "tests\TestTexture2D.h"
#include "tests\TestBatchColor.h"
#include "tests\TestBatchTexture2D.h"
#include "tests\TestBatchDynamicGeometry.h"
#include "tests\TestBatchIndices.h"
#include "tests\TestBatchRenderer.h"

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

	{

		/**
		*
		* blending 混合方式
		* 具体解释：看 /doc/blend-*.jpg
		*/
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		Renderer renderer;

		ImGui::CreateContext();
		ImGui_ImplGlfwGL3_Init(window, true);
		ImGui::StyleColorsDark();

		tests::Test* currentTest = nullptr;
		tests::TestMenu* testMenu = new tests::TestMenu(currentTest);
		currentTest = testMenu;

		testMenu->RegisterTest<tests::TestClearColor>("Clear Color");
		testMenu->RegisterTest<tests::TestTexture2D>("2D Texture");
		testMenu->RegisterTest<tests::TestBatchColor>("Batch Color");
		testMenu->RegisterTest<tests::TestBatchTexture2D>("Batch 2D Texture");
		testMenu->RegisterTest<tests::TestBatchDynamicGeometry>("Batch Dynamic Geometries");
		testMenu->RegisterTest<tests::TestBatchIndices>("Batch Indices");
		testMenu->RegisterTest<tests::TestBatchRenderer>("Batch Renderer");

		while (!glfwWindowShouldClose(window))
		{
			GLCall(glClearColor(0, 0, 0, 1));

			renderer.Clear();

			ImGui_ImplGlfwGL3_NewFrame();
			if (currentTest)
			{
				currentTest->OnUpdate(0.0f);
				currentTest->OnRender();
				ImGui::Begin("Test");
				if (currentTest != testMenu && ImGui::Button("<-"))
				{
					delete currentTest;
					currentTest = testMenu;
				}
				currentTest->OnImGuiRender();
				ImGui::End();
			}

			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		delete currentTest;
		if (currentTest != testMenu)
			delete testMenu;
	}

	// Cleanup
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}
#endif
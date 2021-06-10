/**
* --------------------------------------------------------------------------
* MVP:
* M 模型变换：将物体从局部坐标系变换到世界坐标系，决定物体所在的场景，
*			比如物体是一个沙发，通过模型变换，决定这个沙发到底是在上海，还是在洛杉矶。
* V 视图变换：将相机作为观察点，并决定相机的位置的位置和方向，即世界坐标系变换到观察坐标系
*			比如把相机放置在物体的左边（相当于物体向右边移动）
* P 投影变换：正交投影或透视投影（视锥体，近大远小，FOV视角大小），将物体投影到裁剪空间（近平面），
*			比如物体是一个沙发，你到底是从左边看他，还是右边。使近距离看，还是离远了看，
*			是从上到下看，还是从下往上看，如果里的太近你可能只看得到沙发的一部分。
* ---------------------------------------------------------------------------
* Assimp 库3D模型资源地址
* https://sketchfab.com/
* https://sketchfab.com/sketchpunk/collections/downloads
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "../core/Renderer.h"
#include "../core/VertexBuffer.h"
#include "../core/VertexBufferLayout.h"
#include "../core/IndexBuffer.h"
#include "../core/VertexArray.h"
#include "../core/Shader.h"
#include "../core/Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw_gl3.h"

#include "../tests/TestClearColor.h"
#include "../tests/TestTexture2D.h"
#include "../tests/TestBatchColor.h"
#include "../tests/TestBatchTexture2D.h"
#include "../tests/TestBatchDynamicGeometry.h"
#include "../tests/TestBatchIndices.h"
#include "../tests/TestBatchRenderer.h"
#include "../tests/TestTexture3D.h"
#include "../tests/TestCamera.h"
#include "../tests/TestLight.h"
#include "../tests/TestMesh.h"
#include "../tests/TestDepthTest.h"
#include "../tests/TestStencilTest.h"
#include "../tests/TestBlending.h"

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1200, 600, "Hello OpenGL", NULL, NULL);
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
		//GLCall(glEnable(GL_BLEND));
		//GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		Renderer renderer;

		ImGui::CreateContext();
		ImGui_ImplGlfwGL3_Init(window, true);
		/**
		*
		* 设置字体
		*/
		//ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.FontDefault = io.Fonts->AddFontFromFileTTF("../res/fonts/LongCang-Regular.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());

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
		testMenu->RegisterTest<tests::TestTexture3D>("3D Texture");
		testMenu->RegisterTest<tests::TestCamera>("Camera");
		testMenu->RegisterTest<tests::TestLight>("Light");
		testMenu->RegisterTest<tests::TestMesh>("Assimp Mesh");
		testMenu->RegisterTest<tests::TestDepthTest>("Depth Test");
		testMenu->RegisterTest<tests::TestStencilTest>("Stencil Test");
		testMenu->RegisterTest<tests::TestBlending>("Blending");

		while (!glfwWindowShouldClose(window))
		{
			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			GLCall(glClearColor(0, 0, 0, 1));

			renderer.Clear();

			ImGui_ImplGlfwGL3_NewFrame();
			if (currentTest)
			{
				currentTest->OnUpdate(deltaTime);
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
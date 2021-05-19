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

	//����ˢ�¼��ʱ��
	GLCall(glfwSwapInterval(1));

	std::cout << glfwGetVersionString() << std::endl;

	{

		/**
		*
		* blending ��Ϸ�ʽ
		* ������ͣ��� /doc/blend-*.jpg
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
		testMenu->RegisterTest<tests::TestTexture3D>("3D Texture");

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
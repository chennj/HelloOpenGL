/**
* --------------------------------------------------------------------------
* MVP:
* M ģ�ͱ任��������Ӿֲ�����ϵ�任����������ϵ�������������ڵĳ�����
*			����������һ��ɳ����ͨ��ģ�ͱ任���������ɳ�����������Ϻ�����������ɼ��
* V ��ͼ�任���������Ϊ�۲�㣬�����������λ�õ�λ�úͷ��򣬼���������ϵ�任���۲�����ϵ
*			���������������������ߣ��൱���������ұ��ƶ���
* P ͶӰ�任������ͶӰ��͸��ͶӰ����׶�壬����ԶС��FOV�ӽǴ�С����������ͶӰ���ü��ռ䣨��ƽ�棩��
*			����������һ��ɳ�����㵽���Ǵ���߿����������ұߡ�ʹ�����뿴��������Զ�˿���
*			�Ǵ��ϵ��¿������Ǵ������Ͽ���������̫�������ֻ���õ�ɳ����һ���֡�
* ---------------------------------------------------------------------------
* Assimp ��3Dģ����Դ��ַ
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

	//����ˢ�¼��ʱ��
	GLCall(glfwSwapInterval(1));

	std::cout << glfwGetVersionString() << std::endl;

	{

		/**
		*
		* blending ��Ϸ�ʽ
		* ������ͣ��� /doc/blend-*.jpg
		*/
		//GLCall(glEnable(GL_BLEND));
		//GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		Renderer renderer;

		ImGui::CreateContext();
		ImGui_ImplGlfwGL3_Init(window, true);
		/**
		*
		* ��������
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
#include<GL/glew.h>
#include<iostream>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "vert_array_quick_bind.h"
#include "Global_Bind_Test.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "freecam.h"
#include <functional>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <fstream>
#include <sstream>
#include "artefact.h"

using namespace std;

freecam primary_cam;

void MouseControlWrapper(GLFWwindow* window, double mouse_x, double mouse_y) {
	primary_cam.mouse_handler(window, mouse_x, mouse_y);
}

void ScrollControlWrapper(GLFWwindow* window, double x_disp, double y_disp) {
	primary_cam.scroll_handler(window, x_disp, y_disp);
}

bool FileExists(const std::string& pFile)
{
	Assimp::Importer importer;
	//check if file exists
	std::ifstream fin(pFile.c_str());
	if (!fin.fail()) {
		fin.close();
		cout << "model found at : " << pFile.c_str() << endl;
	}
	else {
		cout << "Couldnt open file: " << pFile.c_str() << endl;
		return false;
	}

	return true;
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1920, 1080, "alien life discovered", glfwGetPrimaryMonitor(), NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glewInit();

	float tetra[] = {
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.f, 0.f, 0.5f, 0.0f,  0.0f, -1.0f
	};

	vector<float> th;
	for (int i = 0;i < 24; i++) {
		th.push_back(tetra[i]);
	}
	float* thd = &th[0];
	unsigned int tetra_indices[] = {
	0, 1, 2,
	2, 3, 1,
	2, 0, 3,
	1, 0, 3
	};

	int count = 0;
	char path[] = "models/metroid/metroid.obj";
	char path2[] = "models/rocket/cool-rocket-painted.obj";
	char path3[] = "models/astroguy/astroguy.obj";
	char path4[] = "models/plane/plane.obj";
	char path5[] = "models/Text/text.obj";
	char path6[] = "models/flag/flag-painted.obj";

	Shader phong_shader("res/shaders/phong_prototype.shader");
	phong_shader.Bind();

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
	glfwSetCursorPosCallback(window, MouseControlWrapper);
	glfwSetScrollCallback(window, ScrollControlWrapper);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	/* Loop until the user closes the window */

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);

	scene_artefact s(path3);
	scene_artefact s1(path);
	scene_artefact s2(path2);
	scene_artefact s3(path4);
	scene_artefact s4(path5);
	scene_artefact s5(path6);

	glm::mat4 model2 = glm::mat4(1.0f);
	model2 = glm::rotate(model2, glm::radians(120.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model2 = glm::translate(model2, glm::vec3(-0.f, -3.0f, -0.f));
	model2 = glm::scale(model2, glm::vec3(1.4f, 1.4f, 1.4f));

	glm::mat4 model3 = glm::mat4(1.0f);
	model3 = glm::scale(model3, glm::vec3(0.25f, 0.25f, 0.25f));

	glm::mat4 model4 = glm::translate(model3, glm::vec3(16.0f, -0.f, -0.f));
	model4 = glm::rotate(model4, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 model5 = glm::translate(model3, glm::vec3(0.0f, -0.f, -0.f));
	model5 = glm::translate(model5, glm::vec3(0.f, -12.0f, 0.f));

	glm::mat4 model6 = glm::mat4(1.0f);
	model6 = glm::translate(model, glm::vec3(-3.0f, 5.f, 0.f));
	model6 = glm::rotate(model6, glm::radians(90.0f), glm::vec3(1.f, 0.f, 0.f));
	model6 = glm::rotate(model6, glm::radians(180.0f), glm::vec3(0.f, 0.f, 1.f));

	glm::mat4 model7 = glm::mat4(1.f);
	model7 = glm::translate(model7, glm::vec3(6.f, -0.5f, -0.f));

	model4 = glm::translate(model4, glm::vec3(0.f, -2.f, 0.f));

	while (!glfwWindowShouldClose(window))
	{
		primary_cam.input_handler(window);
		phong_shader.setUniform4mat4f("view", primary_cam.view());
		phong_shader.setUniform4mat4f("projection", primary_cam.projection());
		phong_shader.setUniform4f("Camera_Position", 0.f, .5f, 0.f, 1.f);
		phong_shader.setUniform4f("light.position", 0.f, 0.5f, 0.f, 1.f);
		phong_shader.setUniform4f("light.ambient", 1.0f, 1.0f, 1.0f, 1.f);
		phong_shader.setUniform4f("light.diffuse", 1.0f, 1.0f, 1.0f, 1.f);
		phong_shader.setUniform4f("light.specular", 1.0f, 1.0f, 1.0f, 1.f);
		/* Render here */

		glClearColor(0.f, 0.f, 0.f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		model = glm::rotate(model, glm::radians(-2.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		s.quick_illustrate_unstable(phong_shader, model3);
		s1.quick_illustrate_unstable(phong_shader, model);
		s2.quick_illustrate_unstable(phong_shader, model2);
		s3.quick_illustrate_unstable(phong_shader, model5);
		s.quick_illustrate_unstable(phong_shader, model4);
		s4.quick_illustrate_unstable(phong_shader, model6);
		s5.quick_illustrate_unstable(phong_shader, model7);
		/* Swap front and ,back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
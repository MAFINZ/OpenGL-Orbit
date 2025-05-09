#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "game.h"
#include "resource_manager.h"

#include <iostream>

#define _USE_MATH_DEFINES
#include <cmath>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

unsigned int SCREEN_WIDTH = 800;
unsigned int SCREEN_HEIGHT = 600;


Game Simulation(SCREEN_WIDTH, SCREEN_HEIGHT);

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "dunno", NULL, NULL);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glViewport(0, 0, 800, 600);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	std::cout << "Shader Init" << std::endl;
	/*
	int nVertices = 32;
	glm::vec3 circlePos[2] = {glm::vec3(-2, 0, 0), glm::vec3(1, 1, 0)};

	float angle = (float) 360.0/nVertices;
	float radius = 1.0f;

	float circle[nVertices*3+3];
	int circleIndices[nVertices*3];

	circle[0] = 0.0f;
	circle[1] = 0.0f;
	circle[2] = 0.0f; 

	for(int i = 0; i < nVertices; i++) {
		circle[(i+1)*3] = radius * sin(glm::radians(angle * (i)));
		circle[(i+1)*3+1] = radius * cos(glm::radians(angle * (i)));
		circle[(i+1)*3+2] = 0.0f;   
		//std::cout << angle * i << std::endl;
		//std::cout << circle[(i+1)*3] << ", " << circle[(i+1)*3+1] << ", " << circle[(i+1)*3+2] << std::endl;
		circleIndices[i*3] = 0;
		circleIndices[i*3+1] = i+1;
		circleIndices[i*3+2] = (i+2) > nVertices ? 1 : i+2;
		//std::cout << circleIndices[i*3] << ", " << circleIndices[i*3+1] << ", " << circleIndices[i*3+2] << std::endl;
	}
	*/
	
	/*for(int i = 0; i < sizeof(sphereVertices)/sizeof(float);) {
		std::cout << i << ": " << sphereVertices[i++] << ", " 
		<< i << ": " << sphereVertices[i++] << ", " 
		<< i << ": " << sphereVertices[i++] << std::endl;
	}*/

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);

	
	Simulation.Init();

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;



	while(!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();

		Simulation.ProcessInput(deltaTime);

		Simulation.Update(deltaTime);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		Simulation.Render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	ResourceManager::Clear();

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if(key >= 0 && key < 1024) {
		if(action == GLFW_PRESS) {
			Simulation.Keys[key] = true;
		} else if (action == GLFW_RELEASE) {
			Simulation.Keys[key] = false;
		}
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	//projection = glm::perspective(glm::radians(70.0f), (float) (width/height), 0.1f, 100.0f);
}
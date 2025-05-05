#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "../../include/stb_image.h"
#include "../../include/shader.h"

#include <iostream>

#define _USE_MATH_DEFINES
#include <cmath>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "dunno", NULL, NULL);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glViewport(0, 0, 800, 600);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	std::cout << "Shader Init" << std::endl;
	Shader shaderProgram("../shaders/shader.vs", "../shaders/shader.fs");
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

	int divisions = 2;
	glm::vec3 circlePos = glm::vec3(0.0f, 0.0f, 0.0f);

	float angleChange = 2*M_PI/6;
	float upAngle = 0.0f;
	float sideAngle = 0.0f;
	float radius = 1.0f;

	float sphereVertices[((divisions*divisions)*3)+6];
	int sphereIndices[(6 + divisions * divisions];
	int nVert = sizeof(sphereVertices)/sizeof(float);

	std::cout << "Sphere size: " << nVert << std::endl;
	std::cout << "Indexes: " << sizeof(sphereIndices)/sizeof(int) << std::endl; 

	sphereVertices[0] = 0.0f,
	sphereVertices[1] = radius;
	sphereVertices[2] = 0.0f;
	sphereVertices[nVert * 3 - 2] = 0.0f,
	sphereVertices[nVert * 3 - 1] = -radius;
	sphereVertices[nVert * 3] = 0.0f;

	for(int i = 0; i < divisions; i++) {
		for(int j = 1; j < divisons-1; j++) {
			sphereVertices[j*3 + (i * 3)] = (radius*cos(angleChange * j)) * sin(angleChange * i); //X
			sphereVertices[j*3 + (i * 3) + 1] = (radius*cos(angleChange * j)) * cos(angleChange * i); //Y
			sphereVertices[j*3 + (i * 3) + 2] = radius * sin(M_PI/2 + angleChange * i); //Z 
		}
	}
	for(int i = 0; i < divisions; i++) {
		//   0  				(013)(123)(234)(924)
		//  1 3
		//  2 4
		//   9
		sphereIndices[i * divisions] = 0; //top triangle
		sphereIndices[i * divisions + 1] = 1 * i * divisions;
		sphereIndices[i * divisions + 2] = 2 * i * divisions;
		for(int j = 1; j < divisions-1; j++) {
			sphereIndices[3 * j] = 1;  		//top left triangle of square
			sphereIndices[3 * j + 1] = 2;
			sphereIndices[3 * j + 2] = 3;
			sphereIndices[3 * j + 3] = 2;	//bottom right triangle of square
			sphereIndices[3 * j + 4] = 3;
			sphereIndices[3 * j + 5] = 4;
		} 	
		sphereIndices[i * 6 + 3] = 9;
		sphereIndices[i * 6 + 4] = 2 * ;
		sphereIndices[i * 6 + 5] = 4;
	}
	

	unsigned int VAO, VBO, EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphereVertices), sphereVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphereIndices), sphereIndices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glm::mat4 model = glm::mat4(1.0);
	glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 5.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 projection = glm::perspective(glm::radians(70.0f), 800.0f/600.0f, 0.1f, 100.0f);
	
	shaderProgram.use();
	int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	int modelView = glGetUniformLocation(shaderProgram.ID, "view");
	glUniformMatrix4fv(modelView, 1, GL_FALSE, glm::value_ptr(view));
	int modelProj = glGetUniformLocation(shaderProgram.ID, "projection");
	glUniformMatrix4fv(modelProj, 1, GL_FALSE, glm::value_ptr(projection));

	while(!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		shaderProgram.use();

        model = glm::mat4(1.0f);
        /*
		glBindVertexArray(VAO);
		model = glm::translate(model, circlePos[0]);
		model = glm::scale(model, glm::vec3(2, 2, 0));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, sizeof(circleIndices)/sizeof(int), GL_UNSIGNED_INT, 0);
		model = glm::mat4(1.0f);
		model = glm::translate(model, circlePos[1]);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, sizeof(circleIndices)/sizeof(int), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		*/
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window) {
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

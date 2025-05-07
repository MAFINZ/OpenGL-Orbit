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

	int divisions = 16;
	glm::vec3 circlePos = glm::vec3(0.0f, 0.0f, 0.0f);

	float vertAngleChange = M_PI/(divisions-1);
	float horiAngleChange = (2*M_PI)/divisions;
	float radius = 1.0f;

	float sphereVertices[((divisions-2)*divisions+2) * 3];
	int sphereIndices[((divisions-3)*2+2) * divisions * 3];
	int lastIndVert = sizeof(sphereVertices)/sizeof(float) - 1;

	std::cout << "Sphere size: " << lastIndVert + 1 << std::endl;
	std::cout << "Indexes: " << sizeof(sphereIndices)/sizeof(int) << std::endl; 

	sphereVertices[0] = 0.0f; //x top vert
	sphereVertices[1] = radius; //y
	sphereVertices[2] = 0.0f; //z
	sphereVertices[lastIndVert-2] = 0.0f; //x bottom vert
	sphereVertices[lastIndVert-1] = -radius; //y
	sphereVertices[lastIndVert] = 0.0f; //z

	int index = 3;
	for(int arc = 0; arc < divisions; arc++) {
		std::cout << "making arc " << arc << std::endl;
		for(int ring = 1; ring <= divisions-2; ring++) {
			std::cout << "making ring " << ring << std::endl;

			sphereVertices[index++] = radius * sin(vertAngleChange * ring) * cos(horiAngleChange*arc);//X
			sphereVertices[index++] = radius * cos(vertAngleChange * ring); //Y
			sphereVertices[index++] = radius * sin(vertAngleChange * ring)* sin(horiAngleChange*arc); //Z 
		}
	}
	index = 0;
	for(int arc = 0; arc < divisions; arc++) {
		// top triangle
		sphereIndices[index++] = 0; // top
		sphereIndices[index++] = 1 + arc * (divisions-2); // bottom left
		sphereIndices[index++] = (arc == divisions - 1) ? 1 : 1 + (arc+1) * (divisions-2); // bottom right
		for(int ring = 0; ring < divisions-3; ring++) {
			/* 0 
			  1 3 5 7
			  2 4 6 8
			       9
			*/
			//top triangle
			sphereIndices[index++] = 1 + ring + (divisions - 2) * arc; //top left
			sphereIndices[index++] = 2 + ring + (divisions - 2) * arc; // bottom left
			sphereIndices[index++] = (arc == divisions - 1) ? 1 + ring : 1 + ring + (divisions - 2) * (arc + 1); //top right
			//bottom triangle
			sphereIndices[index++] = 2 + ring + (divisions - 2) * arc; // bottom left
			sphereIndices[index++] = (arc == divisions - 1) ? 1 + ring : 1 + ring + (divisions - 2) * (arc + 1);
			sphereIndices[index++] = (arc == divisions - 1) ? 2 + ring : 2 + ring + (divisions - 2) * (arc + 1); 
		}
		sphereIndices[index++] = (divisions - 2) + (divisions - 2) * arc; //top left
		sphereIndices[index++] = (arc == divisions - 1) ? (divisions - 2)  : (divisions - 2) + (divisions - 2) * (arc + 1); // top right;
		sphereIndices[index++] = (lastIndVert + 1) / 3 - 1;

	}	
	
	for(int i = 0; i < sizeof(sphereVertices)/sizeof(float);) {
		std::cout << i << ": " << sphereVertices[i++] << ", " 
		<< i << ": " << sphereVertices[i++] << ", " 
		<< i << ": " << sphereVertices[i++] << std::endl;
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
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		shaderProgram.use();

        model = glm::mat4(1.0f);
        
		glBindVertexArray(VAO);
		model = glm::translate(model, circlePos);
		model = glm::rotate(model, (float) (glm::radians(25.0f) * glfwGetTime()), glm::vec3(0.5f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, sizeof(sphereIndices)/sizeof(int), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		
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

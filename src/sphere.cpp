#include <sphere.h>
#include <iostream>

void Sphere::initSphereData() {
	//this->divisions = 12; // minimum 3 
	int divs = this->divisions;
	std::cout << divs << std::endl;
	float vertAngleChange = M_PI/(divs-1);
	float horiAngleChange = (2*M_PI)/divs;
	float rad = this->radius;

	float sphereVertices[((divs-2)*divs+2) * 3];
	//int sphereIndices[indexCount];
	int sphereIndices[((divs-3)*2+2) * divs * 3];
	int lastIndVert = sizeof(sphereVertices)/sizeof(float) - 1;

	std::cout << "Sphere size: " << lastIndVert + 1 << std::endl;
	std::cout << "Indexes: " << this->indexCount << std::endl; 

	sphereVertices[0] = 0.0f; //x top vert
	sphereVertices[1] = rad; //y
	sphereVertices[2] = 0.0f; //z
	sphereVertices[lastIndVert-2] = 0.0f; //x bottom vert
	sphereVertices[lastIndVert-1] = -rad; //y
	sphereVertices[lastIndVert] = 0.0f; //z

	int index = 3;
	for(int arc = 0; arc < divs; arc++) {
		std::cout << "making arc " << arc << std::endl;
		for(int ring = 1; ring <= divs-2; ring++) {
			std::cout << "making ring " << ring << std::endl;

			sphereVertices[index++] = rad * sin(vertAngleChange * ring) * cos(horiAngleChange*arc);//X
			sphereVertices[index++] = rad * cos(vertAngleChange * ring); //Y
			sphereVertices[index++] = rad * sin(vertAngleChange * ring)* sin(horiAngleChange*arc); //Z 
			if (index >= indexCount) {
    std::cerr << "Index overflow at arc " << arc << std::endl;
    break;
}
		}
	}
	
	index = 0;
	for(int arc = 0; arc < divs; arc++) {
		// top triangle
		sphereIndices[index++] = 0; // top
		sphereIndices[index++] = 1 + arc * (divs-2); // bottom left
		sphereIndices[index++] = (arc == divs - 1) ? 1 : 1 + (arc+1) * (divs-2); // bottom right
		for(int ring = 0; ring < divs-3; ring++) {
			/* 0 
			  1 3 5 7
			  2 4 6 8
			       9
			*/
			//top triangle
			sphereIndices[index++] = 1 + ring + (divs - 2) * arc; //top left
			sphereIndices[index++] = 2 + ring + (divs - 2) * arc; // bottom left
			sphereIndices[index++] = (arc == divs - 1) ? 1 + ring : 1 + ring + (divs - 2) * (arc + 1); //top right
			//bottom triangle
			sphereIndices[index++] = 2 + ring + (divs - 2) * arc; // bottom left
			sphereIndices[index++] = (arc == divs - 1) ? 1 + ring : 1 + ring + (divs - 2) * (arc + 1);
			sphereIndices[index++] = (arc == divs - 1) ? 2 + ring : 2 + ring + (divs - 2) * (arc + 1); 
		}
		sphereIndices[index++] = (divs - 2) + (divs - 2) * arc; //top left
		sphereIndices[index++] = (arc == divs - 1) ? (divs - 2)  : (divs - 2) + (divs - 2) * (arc + 1); // top right;
		sphereIndices[index++] = (lastIndVert + 1) / 3 - 1;

	}	
	for(int i = 0; i < sizeof(sphereIndices)/sizeof(int);) {
		std::cout << i << ": " << sphereIndices[i++] << ", " 
		<< i << ": " << sphereIndices[i++] << ", " 
		<< i << ": " << sphereIndices[i++] << std::endl;
	}

	unsigned int VBO, EBO;


	glGenVertexArrays(1, &this->sphereVAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(this->sphereVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphereVertices), sphereVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indexCount * sizeof(int), sphereIndices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);	

}

void Sphere::renderSphere(glm::vec3 position, float rotation, 
			glm::vec3 rotationAxis) {
	this->shader.Use();

    glm::mat4 model = glm::mat4(1.0f);
	int modelLoc = glGetUniformLocation(this->shader.ID, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
      
	glBindVertexArray(this->sphereVAO);
	model = glm::translate(model, position); //remember matrices are applied backwards
	model = glm::rotate(model, (float) (glm::radians(25.0f) * glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    shader.SetMatrix4("model", model);
    glDrawElements(GL_TRIANGLES, this->indexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

Sphere::Sphere(Shader &shader, int divisions, float radius) {
	this->shader = shader;
	this->divisions = divisions;
	this->radius = radius;
	this->indexCount = ((divisions-3)*2+2) * divisions * 3;
	initSphereData();
}

Sphere::~Sphere() {
	if (glIsVertexArray(this->sphereVAO)) {
		std::cout << "deleting vao" << std::endl;
    	glDeleteVertexArrays(1, &this->sphereVAO);
	}
}
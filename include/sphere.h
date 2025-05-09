#ifndef SPHERE_H
#define SPHERE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader.h>
#include <texture.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Sphere {
	public:
		Sphere(Shader &shader, int divisions = 3, float radius = 1.0f);
		~Sphere();

		void renderSphere(glm::vec3 position = glm::vec3(0.0,0.0,0.0), float rotation = 0.0f, 
			glm::vec3 rotationAxis = glm::vec3(0.0, 1.0, 0.0));

	private:
		Shader shader;
		int divisions;
		int radius;
		int indexCount;
		unsigned int sphereVAO;

		void initSphereData();
};

#endif
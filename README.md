# OpenGL-Orbit
Making a planet orbit thingy with opengl or something like that for school coding club or sum

include folder has GLAD, GLFW3, GLM  
Shader header, texture header, resource manager header are from learnopengl.org currently I will make my own sometime later or for a different project

my linux system command line  
`g++ main.cpp game.cpp shader.cpp texture.cpp resource_manager.cpp glad.c -o main -I../include -I../../include -lglfw && ./main`

my windows system command line (outdated)  
`g++ main.cpp glad.c -I../../include -o main -lglfw3 -lopengl32 && ./main`

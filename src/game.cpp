#include "game.h"
#include <resource_manager.h>
#include <sphere.h>

Sphere *sphere;

Game::Game(unsigned int width, unsigned int height) 
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{ 

}

Game::~Game()
{
    
}

void Game::Init()
{
    Shader shaderProgram = ResourceManager::LoadShader("../shaders/shader.vs", "../shaders/shader.fs", nullptr, "shaderProgram");
    
    sphere = new Sphere(shaderProgram, 16, 1.0f);

    glm::mat4 projection = glm::perspective(glm::radians(70.0f), (float) this->Width/ (float)this->Height, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 5.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
    
    shaderProgram.Use();
    shaderProgram.SetMatrix4("view", view);
    shaderProgram.SetMatrix4("projection", projection); 

}

void Game::Update(float dt)
{
    
}

void Game::ProcessInput(float dt)
{
   
}

void Game::Render()
{
    sphere->renderSphere();
}

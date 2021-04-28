#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>



#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include "shader.h"

// Represents the current state of the game
enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

//Shader ourShader;

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game
{
public:


    
    static unsigned int VBO, VAO, EBO, VAO2, Player_texture, Cube_texture, Sky_texture;

    // game state
    GameState               State;
    bool                    Keys[1024];
    unsigned int            Width, Height;
    // constructor/destructor
    Game(unsigned int width, unsigned int height);
    ~Game();

    // initialize game state (load all shaders/textures/levels)
    void Init();






    // game loop
    void ProcessInput(float dt);
    void Update(float dt);
    void DrawPrimitives();
};

#endif
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <vector>
//#include <stb_image.h>
#define STB_IMAGE_IMPLEMENTATION
//#include <stb_image.h>
#include <Shader_Class.h>
#include <iostream>

#include "game.h"


Game ourGame(800, 600);

/////////////////////////////////////////CALL FUNCTIONS//////////////////////////////////
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);


/////////////////////////////////////////GLOBAL VARIABLES//////////////////////////////////
///Screen Variables
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

///DeltaTime Variables
float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

///Camera Variables
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

///Mouse Variables
bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;

///Others Variables
glm::mat4 view;


int main()
{
	/////////////////////////////////////////GLFW INITIALIZE AND CONFIGURATION//////////////////////////////////

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	////////////////////////////////////////////GLFW WINDOW CREATION////////////////////////////////////////////

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "The Project", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	/////////////////////////////////////////CALL SHADER PROGRAMS//////////////////////////////////

	Shader ourShader("VertexShader.glsl", "FragmentShader.glsl");

	/////////////////////////////////////////SET VERTEX ATTRIBUTES AND GENERATE AND BIND ARRAYS AND BUFFERS//////////////////////////////////

	//@@DRAWPRIMITIVES() de game
	ourGame.DrawPrimitives();


//	/////////////////////////////////////////////////////////////////////SHADER ACTIVATION
//
	ourShader.use(); // don’t forget to activate the shader first!

	glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0); //get the uniform location (MANUALLY)
	glUniform1i(glGetUniformLocation(ourShader.ID, "texture2"), 1); //get the uniform location (MANUALLY)
	glUniform1i(glGetUniformLocation(ourShader.ID, "texture3"), 2);	//get the uniform location (MANUALLY)
//
//
//
//
//	//MORE CUBES
	struct Cube {
		glm::vec3 position;
		GLuint texture_id;
	};


	Cube cubes[11] = {
	{ glm::vec3(0.0f, 0.0f,  0.0f),  Game::Cube_texture },
	{ glm::vec3(1.0f, 0.0f, -15.0f), Game::Cube_texture },
	{ glm::vec3(2.0f, 0.0f, -2.5f),  Game::Cube_texture },
	{ glm::vec3(3.0,  0.0f, -12.3f), Game::Cube_texture },
	{ glm::vec3(4.0f, 0.0f, -3.5f),  Game::Cube_texture },
	{ glm::vec3(5.0f, 0.0f, -7.5f),  Game::Cube_texture },
	{ glm::vec3(6.0f, 0.0f, -2.5f),  Game::Cube_texture },
	{ glm::vec3(7.0f, 0.0f, -2.5f),  Game::Cube_texture },
	{ glm::vec3(8.0f, 0.0f, -1.5f),  Game::Cube_texture },
	{ glm::vec3(9.0f, 0.0f, -1.5f),  Game::Cube_texture },
	{ glm::vec3(9.0f, 1.0f, -1.5f),  Game::Cube_texture }
	};





	////////////////////////////////////////////////////////////////////



	while (!glfwWindowShouldClose(window))           /////////////////////////////////////////RENDER LOOP//////////////////////////////////
	{


		// input		
		processInput(window);

		// render		
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		ourShader.use(); //SHADER ACTIVATION

		///////////////////////////////////SKYBOX RENDERING
		glBindVertexArray(Game::VAO2);
		//glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, Game::Sky_texture);
		glDrawArrays(GL_TRIANGLES, 0, 36);



		///////////////////////////////////PLAYER RENDERING
		glBindVertexArray(Game::VAO);
		//glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Game::Player_texture);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		////////////////////////////////// CUBES RENDERING
		for (unsigned int i = 0; i < 11; i++)
		{
			//glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, cubes[i].texture_id);
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubes[i].position);
			float angle = 20.0f * i;
			ourShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}



		///////////////////////////////////////MATRICES and TRANSFORMS

		glm::mat4 model = glm::mat4(1.0f);
		/*model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.0f));
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));*/
		int modelLoc = glGetUniformLocation(ourShader.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ourShader.setMat4("model", model);

		///////////////////////////////////////Call Binds (For CAMERA "View Matrix")		
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		int modelLoc2 = glGetUniformLocation(ourShader.ID, "view");
		glUniformMatrix4fv(modelLoc2, 1, GL_FALSE, glm::value_ptr(view));
		ourShader.setMat4("view", view);


		///////////////////////////////////////Call Binds (For PROJECTION)
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		int modelLoc3 = glGetUniformLocation(ourShader.ID, "projection");
		glUniformMatrix4fv(modelLoc3, 1, GL_FALSE, glm::value_ptr(projection));
		ourShader.setMat4("projection", projection);


		/////////////////////////////////////////UPDATE BUFFERS AND EVENT QUEUE//////////////////////////////////////////////////////
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	/////////////////////////////////////////DELETE EVERYTHING WHEN LOOP IS OVER//////////////////////////////////////////////////////
	glDeleteVertexArrays(1, &Game::VAO);
	glDeleteBuffers(1, &Game::VBO);
	glDeleteBuffers(1, &Game::EBO);
	/////////////////////////////////////////TERMINATE PROGRAM WHEN LOOPS IS OVER//////////////////////////////////////////////////////
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{

	float cameraSpeed = 2.5f * deltaTime;


	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) *
		cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) *
		cameraSpeed;
}


/////////////////////////////////////////INPUT FUNCTIONS DEFINITIONS//////////////////////////////////////////////////////
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov -= (float)yoffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
}




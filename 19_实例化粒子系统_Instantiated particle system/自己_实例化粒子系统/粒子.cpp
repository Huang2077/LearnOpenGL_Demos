#define GLEW_STATIC
#include <iostream>
#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/SOIL.h>
#include <GL/SHADER.h>
#include <vector>
#include "Particle.h"
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glew32s.lib") 
#pragma comment (lib, "glfw3.lib")
#pragma comment (lib, "SOIL.lib")
#define GLM_FORCE_RADIANS  //Bug

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Window dimensions
const GLuint WIDTH = 1000, HEIGHT = 600;
int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Particle", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, WIDTH, HEIGHT);
	glEnable(GL_PROGRAM_POINT_SIZE);



	//设置粒子的VAO传入初始位置给GPU
	GLfloat Position[] = {
    //position        v0                inittime
	-0.5f, 0.0f, 0.0f, 0.0f, 0.3f, 0.0f, 0.0f,
	-0.5f, 0.0f, 0.0f, 0.05f, 0.3f, 0.0f, 0.0f,
	-0.5f, 0.0f, 0.0f, 0.07f, 0.3f, 0.0f, 0.0f,
	-0.5f, 0.0f, 0.0f, 0.09f, 0.3f, 0.0f, 0.0f,
	-0.5f, 0.0f, 0.0f, 0.11f, 0.3f, 0.0f, 0.0f,
	-0.5f, 0.0f, 0.0f, 0.13f, 0.3f, 0.0f, 0.0f,
	-0.5f, 0.0f, 0.0f, 0.15f, 0.3f, 0.0f, 0.0f,
	-0.5f, 0.0f, 0.0f, 0.17f, 0.3f, 0.0f, 0.0f,
	-0.5f, 0.0f, 0.0f, 0.19f, 0.3f, 0.0f, 0.0f,
	-0.5f, 0.0f, 0.0f, 0.21f, 0.3f, 0.0f, 0.0f,
	-0.5f, 0.0f, 0.0f, 0.23f, 0.3f, 0.0f, 0.0f
	};
	
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Position), Position, GL_STATIC_DRAW);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0); // Unbind VAO

	//创建shader
	Shader ParticleShader("Particle.vs", "Particle.frag");


	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//开始渲染
		ParticleShader.Use();
		float now_time = glfwGetTime();//获得当前时间
		glm::vec3 a = glm::vec3(0.0, -0.1, 0.0);
		glUniform3f(glGetUniformLocation(ParticleShader.Program, "a"), a.x, a.y, a.z);
		glUniform1f(glGetUniformLocation(ParticleShader.Program, "now_time"), now_time);
		glBindVertexArray(VAO);
		glDrawArrays(GL_POINTS, 0, 10);
		glBindVertexArray(0);
		glfwSwapBuffers(window);
	}
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}
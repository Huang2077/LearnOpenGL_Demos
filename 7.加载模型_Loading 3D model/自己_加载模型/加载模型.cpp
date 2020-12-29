#define GLEW_STATIC
#include <iostream>
#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/glm.hpp>
#include <GL/CAMERA.h>
#include <GL/SHADER.h>
#include <GL/SOIL.h>
#include "Model.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glew32s.lib") 
#pragma comment (lib, "glfw3.lib")
#pragma comment (lib, "SOIL.lib")
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void do_movement();


// Window dimensions
const GLuint WIDTH = 1500, HEIGHT = 1000;
glm::vec3 lightColor = glm::vec3(0.0f, 0.0f, 1.0f);
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool    keys[1024];
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

							// Shaders



							// The MAIN function, from here we start the application and run the game loop
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
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);


	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	// Define the viewport dimensions
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);



	// 创建world_shader//////////////////////////////////////////////
	Shader worldProgram = Shader("world.vs", "world.frag");
   
	//加载模型/////////////////////////
	Model ourmodel("planet/planet.obj");



	




	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		GLfloat nowtime = glfwGetTime();
		deltaTime = nowtime - lastFrame;
		lastFrame = nowtime;
		glfwPollEvents();
		do_movement();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		int i;
		// Use cooresponding shader when setting uniforms/drawing objects
		worldProgram.Use();

		GLint cameraPosLoc = glGetUniformLocation(worldProgram.Program, "cameraPos");
		glUniform3f(cameraPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);


	


		//传平行光元素
		GLint DirLightAmbientLoc = glGetUniformLocation(worldProgram.Program, "dirlight.ambient");
		glUniform3f(DirLightAmbientLoc, 0.2f, 0.2f, 0.2f);
		GLint DirLightDiffuseLoc = glGetUniformLocation(worldProgram.Program, "dirlight.diffuse");
		glUniform3f(DirLightDiffuseLoc, 0.5f, 0.5f, 0.5f);// 让我们把这个光调暗一点，这样会看起来更自然
		GLint DirLightSpecularLoc = glGetUniformLocation(worldProgram.Program, "dirlight.specular");
		glUniform3f(DirLightSpecularLoc, 1.0f, 1.0f, 1.0f);
		GLint DirLightDirctionLoc = glGetUniformLocation(worldProgram.Program, "dirlight.derection");
		glUniform3f(DirLightDirctionLoc, -0.2f, -1.0f, -0.3f);


		// 传点光元素
		glUniform3f(glGetUniformLocation(worldProgram.Program, "pointLights.position"), 2.0f, 2.0f,2.0f );
		glUniform3f(glGetUniformLocation(worldProgram.Program, "pointLights.ambient"), 0.2f, 0.2f, 0.2f);
		glUniform3f(glGetUniformLocation(worldProgram.Program, "pointLights.diffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(worldProgram.Program, "pointLights.specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(worldProgram.Program, "pointLights.Kc"), 1.0f);
		glUniform1f(glGetUniformLocation(worldProgram.Program, "pointLights.Kl"), 0.09);
		glUniform1f(glGetUniformLocation(worldProgram.Program, "pointLights.Kq"), 0.032);
	



		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(camera.Zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(worldProgram.Program, "model");
		GLint viewLoc = glGetUniformLocation(worldProgram.Program, "view");
		GLint projLoc = glGetUniformLocation(worldProgram.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			
		ourmodel.Draw(worldProgram);


		glfwSwapBuffers(window);
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}

// Is called whenever a key is pressed/released via GLFW
// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void do_movement()
{
	// Camera controls
	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
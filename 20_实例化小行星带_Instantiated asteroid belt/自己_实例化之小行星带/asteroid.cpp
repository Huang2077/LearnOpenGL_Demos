#define GLEW_STATIC
#include <iostream>
#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/glm.hpp>
#include <GL/CAMERA.h>
#include <GL/SHADER.h>
#include <GL/SOIL.h>
#include <GL/Model.h>
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
Camera  camera(glm::vec3(0.0f, 25.0f, 55.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool    keys[1024];
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

GLfloat displacement_x[2000];
GLfloat displacement_y[2000];
GLfloat displacement_z[2000];
GLfloat S[2000];
GLfloat rotAngle[2000];

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
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "asteroid", nullptr, nullptr);
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
	Shader PlanetShader = Shader("planet.vs", "planet.frag");
	Shader AsteroidShader = Shader("asteroid.vs", "asteroid.frag");

	//加载模型/////////////////////////
	Model planet("planet/planet.obj");
	Model asteroid("rock/rock.obj");

	
	for (int i = 0; i < 2000; i++)
	{
		displacement_y[i]  = (rand() % (GLint)(2 * 2.5 * 100)) / 100.0f - 2.5;
		displacement_x[i] = (rand() % (GLint)(2 * 2.5 * 100)) / 100.0f - 2.5;
		displacement_z[i] = (rand() % (GLint)(2 * 2.5 * 100)) / 100.0f - 2.5;
		//S[i] = 0.2f;
		rotAngle[i] = (rand() % 360);
	}

	GLfloat AG = 0;
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
		//
		
	//初始化小行星位移model矩阵
	GLuint amount = 2000;
	glm::mat4* modelMatrices;
	modelMatrices = new glm::mat4[amount];
	GLfloat radius = 50.0;
	GLfloat offset = 2.5f;
	AG = AG + 0.001f;
	if (AG >= 360.0)
		AG = 0.0f;
		for (GLuint i = 0; i < amount; i++)
		{
			glm::mat4 model;
			// 1. Translation: displace along circle with 'radius' in range [-offset, offset]
			GLfloat angle = (GLfloat)i / (GLfloat)amount * 360.0f + AG;
			GLfloat new_r = sqrt(pow(radius + displacement_x[i], 2) + pow(radius + displacement_z[i], 2));
			new_r /= 1.5;
			GLfloat x = sin(angle) * new_r;
			GLfloat y = displacement_y[i] * 0.4f; // y value has smaller displacement
			GLfloat z = cos(angle) * new_r;
			model = glm::translate(model, glm::vec3(x, y, z));

			// 2. Scale: Scale between 0.05 and 0.25f	
			GLfloat scale;
			switch (i%5)
			{
			case 0: model = glm::scale(model, glm::vec3(0.2));
			case 1: model = glm::scale(model, glm::vec3(0.3));
			case 2: model = glm::scale(model, glm::vec3(0.25));
			case 3: model = glm::scale(model, glm::vec3(0.23));
			case 4: model = glm::scale(model, glm::vec3(0.2));
			default:
				break;
			}
			// 3. Rotation: add random rotation around a (semi)randomly picked rotation axis vector
			model = glm::rotate(model, rotAngle[i]+AG*1000, glm::vec3(displacement_x[i], displacement_y[i], displacement_z[i]));
			// 4. Now add to list of matrices
			modelMatrices[i] = model;
		}

		//修改小行星模型的VAO,加上实例数组
		for (GLuint i = 0; i < asteroid.meshes.size(); i++)
		{
			GLuint VAO = asteroid.meshes[i].VAO;
			// Vertex Buffer Object
			GLuint buffer;
			glBindVertexArray(VAO);
			glGenBuffers(1, &buffer);
			glBindBuffer(GL_ARRAY_BUFFER, buffer);
			glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
			// Vertex Attributes
			GLsizei vec4Size = sizeof(glm::vec4);
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)0);
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)(vec4Size));
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)(2 * vec4Size));
			glEnableVertexAttribArray(6);
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)(3 * vec4Size));

			glVertexAttribDivisor(3, 1);
			glVertexAttribDivisor(4, 1);
			glVertexAttribDivisor(5, 1);
			glVertexAttribDivisor(6, 1);

			glBindVertexArray(0);
		}

		//开始渲染行星
		PlanetShader.Use();
		//向shader传递view.projection矩阵
		glm::mat4 view;
		view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(camera.Zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 500.0f);
		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(PlanetShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(PlanetShader.Program, "view");
		GLint projLoc = glGetUniformLocation(PlanetShader.Program, "projection");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, -5.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		planet.Draw(PlanetShader);

		//渲染小行星带
		AsteroidShader.Use();
		GLint viewLoc1 = glGetUniformLocation(AsteroidShader.Program, "view");
		GLint projLoc1 = glGetUniformLocation(AsteroidShader.Program, "projection");
		glUniformMatrix4fv(viewLoc1, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc1, 1, GL_FALSE, glm::value_ptr(projection));
		glBindTexture(GL_TEXTURE_2D, asteroid.textures_loaded[0].id); 
		for (GLuint i = 0; i < asteroid.meshes.size(); i++)
		{
			glBindVertexArray(asteroid.meshes[i].VAO);
			glDrawElementsInstanced(
				GL_TRIANGLES, asteroid.meshes[i].vertices.size(), GL_UNSIGNED_INT, 0, amount
			);
			glBindVertexArray(0);
		}


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


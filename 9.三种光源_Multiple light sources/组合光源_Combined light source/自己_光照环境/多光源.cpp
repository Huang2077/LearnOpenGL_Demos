#define GLEW_STATIC
#include <iostream>
#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/glm.hpp>
#include <GL/FPS_CAMERA.h>
#include <GL/SHADER.h>
#include <GL/SOIL.h>
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
Camera  camera(glm::vec3(0.0f, 5.0f, 3.0f));
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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED|GLFW_CURSOR_HIDDEN);


	// ����world_shader//////////////////////////////////////////////
	Shader worldProgram = Shader("world.vs", "world.frag");
	//////����light_shader//////////////////////////////////////////////////////////////////////////////
	Shader lightProgram = Shader("light.vs", "light.frag");


	// Set up vertex data (and buffer(s)) and attribute pointers
   //���ö�������/////////////////////////////////////////////////////////////////////////////////////
	float vertices[] = {
		// Positions          // Normals           // Texture Coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};
	GLuint VBO, worldVAO;  //�����������VBO��VAO
	glGenVertexArrays(1, &worldVAO);//��VAO�Ĺ���ID�����
	glGenBuffers(1, &VBO);//��VBO�Ĺ���ID����һ�����
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(worldVAO);//��VAO�󶨵���������
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //��VBO�󶨵����㻺����
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);//���ڴ��еĶ������鴫���Դ��еĶ��㻺�����������ж��㻺�����VBO��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);//���ö�������ָ�롢������ɫ�����ո�ָ�뷽ʽ�Ӷ��㻺��������ȡ���ݵ��Լ��Ķ���������
	glEnableVertexAttribArray(0);//ʹ�øö�������ָ��
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);//ʹ�øö�������ָ��
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);//ʹ�øö�������ָ��
	glBindBuffer(GL_ARRAY_BUFFER, 0);// ��󶥵㻺������VBO
	glBindVertexArray(0); // ���VAO

	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// ֻ��Ҫ��VBO�����ٴ�����VBO�����ݣ���Ϊ����(����)��VBO�������Ѿ���������ȷ�������嶥������
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// ���õ�������Ķ�������ָ��(�����õƵĶ�������)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	//��������/////////////////////////////////////////////
	GLuint diffuseMap, specularMap;
	glGenTextures(1, &diffuseMap);
	glGenTextures(1, &specularMap);
	int i_width, i_height;
	unsigned char* image;
	// Diffuse map
	image = SOIL_load_image("container2.png", &i_width, &i_height, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, i_width, i_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	// Specular map
	image = SOIL_load_image("container2_specular.png", &i_width, &i_height, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, specularMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, i_width, i_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);


	
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, 3.3f, 4.0f),
		glm::vec3(4.0f,  2.0f, 12.0f),
		glm::vec3(5.0f,  6.0f, 3.0f)
	};

	glm::vec3  cubePositions[10] = {
		glm::vec3(1.0f,1.0f,1.0f),
		glm::vec3(12.0f,1.0f,5.0f),
		glm::vec3(6.0f,3.1f,6.0f),
		glm::vec3(2.0f,3.2f,7.0f),
		glm::vec3(3.0f,5.1f,8.0f),
		glm::vec3(4.0f,9.2f,9.0f),
		glm::vec3(2.5f,7.6f,5.6f),
		glm::vec3(6.6f,8.9f,4.4f),
		glm::vec3(1.7f,1.03f,3.3f),
		glm::vec3(8.3f,3.3f,8.8f),
	};


	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		GLfloat nowtime = glfwGetTime();
		deltaTime = nowtime - lastFrame;
		lastFrame = nowtime;
		glfwPollEvents();
		do_movement();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		int i;
		// Use cooresponding shader when setting uniforms/drawing objects
		worldProgram.Use();

		GLint cameraPosLoc = glGetUniformLocation(worldProgram.Program, "cameraPos");
		glUniform3f(cameraPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);

	
	    //������
		GLint matShininessLoc = glGetUniformLocation(worldProgram.Program, "material.shininess");
		glUniform1i(glGetUniformLocation(worldProgram.Program, "material.diffuse"), 0);
		glUniform1i(glGetUniformLocation(worldProgram.Program, "material.specular"), 1);
		glUniform1f(matShininessLoc, 32.0f);


		
		//��ƽ�й�Ԫ��
		GLint DirLightAmbientLoc = glGetUniformLocation(worldProgram.Program, "dirlight.ambient");
		glUniform3f(DirLightAmbientLoc, 0.05f, 0.05f, 0.05f);
		GLint DirLightDiffuseLoc = glGetUniformLocation(worldProgram.Program, "dirlight.diffuse");
		glUniform3f(DirLightDiffuseLoc, 0.1f, 0.1f, 0.1f);// �����ǰ���������һ�㣬�����ῴ��������Ȼ
		GLint DirLightSpecularLoc = glGetUniformLocation(worldProgram.Program, "dirlight.specular");
		glUniform3f(DirLightSpecularLoc, 1.0f, 1.0f, 1.0f);
		GLint DirLightDirctionLoc = glGetUniformLocation(worldProgram.Program, "dirlight.derection");
		glUniform3f(DirLightDirctionLoc, -0.2f, -1.0f, -0.3f);

	
		// Point light 1
		glUniform3f(glGetUniformLocation(worldProgram.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(worldProgram.Program, "pointLights[0].ambient"), 0.0f, 0.0f, 0.05f);
		glUniform3f(glGetUniformLocation(worldProgram.Program, "pointLights[0].diffuse"), 0.0f, 0.0f, 0.8f);
		glUniform3f(glGetUniformLocation(worldProgram.Program, "pointLights[0].specular"), 0.0f, 0.0f, 1.0f);
		glUniform1f(glGetUniformLocation(worldProgram.Program, "pointLights[0].Kc"), 1.0f);
		glUniform1f(glGetUniformLocation(worldProgram.Program, "pointLights[0].Kl"), 0.09);
		glUniform1f(glGetUniformLocation(worldProgram.Program, "pointLights[0].Kq"), 0.032);
		// Point light 2
		glUniform3f(glGetUniformLocation(worldProgram.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(worldProgram.Program, "pointLights[1].ambient"), 0.0f, 0.0f, 0.05f);
		glUniform3f(glGetUniformLocation(worldProgram.Program, "pointLights[1].diffuse"), 0.0f, 0.0f, 0.8f);
		glUniform3f(glGetUniformLocation(worldProgram.Program, "pointLights[1].specular"), 0.0f, 0.0f, 1.0f);
		glUniform1f(glGetUniformLocation(worldProgram.Program, "pointLights[1].Kc"), 1.0f);
		glUniform1f(glGetUniformLocation(worldProgram.Program, "pointLights[1].Kl"), 0.09);
		glUniform1f(glGetUniformLocation(worldProgram.Program, "pointLights[1].Kq"), 0.032);
		// Point light 3
		glUniform3f(glGetUniformLocation(worldProgram.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(worldProgram.Program, "pointLights[2].ambient"), 0.0f, 0.0f, 0.05f);
		glUniform3f(glGetUniformLocation(worldProgram.Program, "pointLights[2].diffuse"), 0.0f, 0.0f, 0.8f);
		glUniform3f(glGetUniformLocation(worldProgram.Program, "pointLights[2].specular"), 0.0f, 0.0f, 1.0f);
		glUniform1f(glGetUniformLocation(worldProgram.Program, "pointLights[2].Kc"), 1.0f);
		glUniform1f(glGetUniformLocation(worldProgram.Program, "pointLights[2].Kl"), 0.09);
		glUniform1f(glGetUniformLocation(worldProgram.Program, "pointLights[2].Kq"), 0.032);
		// Point light 4
		glUniform3f(glGetUniformLocation(worldProgram.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(worldProgram.Program, "pointLights[3].ambient"), 0.0f, 0.0f, 0.05f);
		glUniform3f(glGetUniformLocation(worldProgram.Program, "pointLights[3].diffuse"), 0.0f, 0.0f, 0.8f);
		glUniform3f(glGetUniformLocation(worldProgram.Program, "pointLights[3].specular"), 0.0f, 0.0f, 1.0f);
		glUniform1f(glGetUniformLocation(worldProgram.Program, "pointLights[3].Kc"), 1.0f);
		glUniform1f(glGetUniformLocation(worldProgram.Program, "pointLights[3].Kl"), 0.09);
		glUniform1f(glGetUniformLocation(worldProgram.Program, "pointLights[3].Kq"), 0.032);
	    //���۹������
		GLint ConeLightAmbientLoc = glGetUniformLocation(worldProgram.Program, "conelight.ambient");
		GLint ConeLightDiffuseLoc = glGetUniformLocation(worldProgram.Program, "conelight.diffuse");
		GLint ConeLightSpecularLoc = glGetUniformLocation(worldProgram.Program, "conelight.specular");
		GLint ConeLightPositionLoc = glGetUniformLocation(worldProgram.Program, "conelight.position");
		GLint ConeLightDirectionLoc = glGetUniformLocation(worldProgram.Program, "conelight.direction");
		GLint ConeLightCutoffLoc = glGetUniformLocation(worldProgram.Program, "conelight.cutoff");
		GLint ConeLightOutcutoffLoc = glGetUniformLocation(worldProgram.Program, "conelight.outcutoff");

		glUniform3f(ConeLightAmbientLoc, 0.1f, 0.1f, 0.1f);
		glUniform3f(ConeLightDiffuseLoc, 0.8f, 0.8f, 0.8f);// �����ǰ���������һ�㣬�����ῴ��������Ȼ
		glUniform3f(ConeLightSpecularLoc, 1.0f, 1.0f, 1.0f);
		glUniform3f(ConeLightPositionLoc, camera.Position.x, camera.Position.y, camera.Position.z);
		glUniform3f(ConeLightDirectionLoc, camera.Front.x, camera.Front.y, camera.Front.z);
		glUniform1f(ConeLightCutoffLoc, glm::cos(glm::radians(10.5f)));
		glUniform1f(ConeLightOutcutoffLoc, glm::cos(glm::radians(15.5f)));



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
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		// Bind specular map
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);
		// Draw the container (using container's vertex attributes)
		glBindVertexArray(worldVAO);
		for (GLuint i = 0; i < 10; i++)
		{
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			GLfloat angle = 20.0f * i;
			model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);

		// Also draw the lamp object, again binding the appropriate shader
		lightProgram.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lightProgram.Program, "model");
		viewLoc = glGetUniformLocation(lightProgram.Program, "view");
		projLoc = glGetUniformLocation(lightProgram.Program, "projection");
		GLint lightColorLoc = glGetUniformLocation(lightProgram.Program, "lightColor");
		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniform3f(lightColorLoc, lightColor.x, lightColor.y, lightColor.z);
	



		// Draw the light object (using light's vertex attributes)
		glBindVertexArray(lightVAO);
		for (i = 0; i < 4; i++)
		{
			glm::mat4 model;
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteVertexArrays(1, &worldVAO);
	glDeleteBuffers(1, &VBO);
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
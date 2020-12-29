#define GLEW_STATIC
#include <iostream>
#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/glm.hpp>
#include <GL/CAMERA.h>
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
GLuint loadTexture(GLchar* path, GLboolean alpha);//����������,����1��·��������2���Ƿ���alphaֵ
const GLuint WIDTH = 1000, HEIGHT = 1000;
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool    keys[1024];
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

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
	glEnable(GL_DEPTH_TEST );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



	// ����world_shader//////////////////////////////////////////////
	Shader worldProgram = Shader("world.vs", "world.frag");


	//���õذ�Ķ����������Ӧ��VAO,VBO///////////////////////
	float floor_vertices[] = {
		// Positions               // Texture Coords
		-0.5f, -0.5f, -0.5f,  0.0f, 5.0f,
		0.5f, -0.5f, -0.5f,  5.0f, 5.0f,
		0.5f, -0.5f,  0.5f,  5.0f, 0.0f,

		0.5f, -0.5f,  0.5f,  5.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 5.0f

	};
	GLuint floorVBO, floorVAO;  //�����������VBO��VAO
	glGenVertexArrays(1, &floorVAO);//��VAO�Ĺ���ID�����
	glGenBuffers(1, &floorVBO);//��VBO�Ĺ���ID����һ�����
						  // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(floorVAO);//��VAO�󶨵���������
	glBindBuffer(GL_ARRAY_BUFFER, floorVBO); //��VBO�󶨵����㻺����
	glBufferData(GL_ARRAY_BUFFER, sizeof(floor_vertices), floor_vertices, GL_STATIC_DRAW);//���ڴ��еĶ������鴫���Դ��еĶ��㻺�����������ж��㻺�����VBO��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);//���ö�������ָ�롢������ɫ�����ո�ָ�뷽ʽ�Ӷ��㻺��������ȡ���ݵ��Լ��Ķ���������
	glEnableVertexAttribArray(0);//ʹ�øö�������ָ��
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);//ʹ�øö�������ָ��
	glBindBuffer(GL_ARRAY_BUFFER, 0);// ��󶥵㻺������VBO
	glBindVertexArray(0); // ���VAO


	

//�����������VAO.VBO/////////////
	float object_vertices[] = {
		// Back face
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right         
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
    // Front face
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
    // Left face
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
    // Right face
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right         
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
    // Bottom face
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
    // Top face
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left        
	};
	GLuint objectVBO, objectVAO;  //�����������VBO��VAO
	glGenVertexArrays(1, &objectVAO);//��VAO�Ĺ���ID�����
	glGenBuffers(1, &objectVBO);//��VBO�Ĺ���ID����һ�����
							   // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(objectVAO);//��VAO�󶨵���������
	glBindBuffer(GL_ARRAY_BUFFER, objectVBO); //��VBO�󶨵����㻺����
	glBufferData(GL_ARRAY_BUFFER, sizeof(object_vertices), object_vertices, GL_STATIC_DRAW);//���ڴ��еĶ������鴫���Դ��еĶ��㻺�����������ж��㻺�����VBO��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);//���ö�������ָ�롢������ɫ�����ո�ָ�뷽ʽ�Ӷ��㻺��������ȡ���ݵ��Լ��Ķ���������
	glEnableVertexAttribArray(0);//ʹ�øö�������ָ��
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);//ʹ�øö�������ָ��
	glBindBuffer(GL_ARRAY_BUFFER, 0);// ��󶥵㻺������VBO
	glBindVertexArray(0); 

	//����С�ݵĶ������顢VAO��VBO///////////
	float grass_vertices[] = {
		0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
		0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

		0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
		1.0f,  0.5f,  0.0f,  1.0f,  0.0f
	};
	GLuint grassVBO, grassVAO;  //�����������VBO��VAO
	glGenVertexArrays(1, &grassVAO);//��VAO�Ĺ���ID�����
	glGenBuffers(1, &grassVBO);//��VBO�Ĺ���ID����һ�����
								// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(grassVAO);//��VAO�󶨵���������
	glBindBuffer(GL_ARRAY_BUFFER, grassVBO); //��VBO�󶨵����㻺����
	glBufferData(GL_ARRAY_BUFFER, sizeof(grass_vertices), grass_vertices, GL_STATIC_DRAW);//���ڴ��еĶ������鴫���Դ��еĶ��㻺�����������ж��㻺�����VBO��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);//���ö�������ָ�롢������ɫ�����ո�ָ�뷽ʽ�Ӷ��㻺��������ȡ���ݵ��Լ��Ķ���������
	glEnableVertexAttribArray(0);//ʹ�øö�������ָ��
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);//ʹ�øö�������ָ��
	glBindBuffer(GL_ARRAY_BUFFER, 0);// ��󶥵㻺������VBO
	glBindVertexArray(0);


  //�������������/////////////////////////////////////////////
	GLuint cube_texture = loadTexture("2.jpg",false);//����������	
	GLuint floor_texture = loadTexture("3.jpg",false);//��������
	GLuint grass_texture = loadTexture("grass.png", true);//�ݵ������alphaֵ

	

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

		// Use cooresponding shader when setting uniforms/drawing objects
		worldProgram.Use();

		glm::mat4 view;
		view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(camera.Zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(worldProgram.Program, "model");
		GLint viewLoc = glGetUniformLocation(worldProgram.Program, "view");
		GLint projLoc = glGetUniformLocation(worldProgram.Program, "projection");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	


		//��Ⱦ�ذ�
		//����ذ�����Ԫ
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, floor_texture);
		glUniform1i(glGetUniformLocation(worldProgram.Program, "ourTexture"), 0);
		glBindVertexArray(floorVAO);
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, -0.01f, 0.0f));//�õذ�����ƽ��һ�㣬������ȳ�ͻ
		model = glm::scale(model, glm::vec3(5.0f, 1.0f, 5.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		//��Ⱦ������
		//�������޳�
		glEnable(GL_CULL_FACE);
		//������������Ԫ
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cube_texture);
		glUniform1i(glGetUniformLocation(worldProgram.Program, "ourTexture"), 0);
		glBindVertexArray(objectVAO);
		glm::mat4 model1;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model1));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		//�ر����޳�
		glDisable(GL_CULL_FACE);

		//��ȾС��
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, grass_texture);
		glUniform1i(glGetUniformLocation(worldProgram.Program, "ourTexture"), 0);
		glBindVertexArray(grassVAO);
		glm::mat4 model2;
		model2 = glm::scale(model2, glm::vec3(0.5f, 0.5f, 1.0f));
		model2 = glm::translate(model2, glm::vec3(0.0f, -0.5f, 1.0f));//�õذ�����ƽ��һ�㣬������ȳ�ͻ
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &objectVAO);
	glDeleteVertexArrays(1, &floorVAO);
	//glDeleteBuffers(1, &floorVBO);
	//glDeleteBuffers(1, &objectVBO);
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

GLuint loadTexture(GLchar* path, GLboolean alpha)
{
	GLuint texture;
	/////////////////////////
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//�Ŵ����ڽ�����Ϊ���ܿ������ص�
																	 // Load image, create texture and generate mipmaps
	int txture_width, txture_height;
	unsigned char* image = SOIL_load_image(path, &txture_width, &txture_height, 0, alpha? SOIL_LOAD_RGBA: SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, alpha ? GL_RGBA : GL_RGB, txture_width, txture_height, 0, alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	return texture;
}


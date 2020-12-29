// Dynamic environment mapping
//2018-3-29
//written by huangjingtao
/* 
This project is using framebuffer and cubetexture to generate Dynamic environoment mapping .
I take enviroment of six direction of metal object to six framebuffer,and used the six framebuffer to creat a cubetexture.
then  the metal object  calculate  enviroment relection.
*/
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
#include <vector>
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glew32s.lib") 
#pragma comment (lib, "glfw3.lib")
#pragma comment (lib, "SOIL.lib")
using namespace std;

//函数区
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void do_movement();
GLuint loadTexture(GLchar* path, GLboolean alpha);
GLuint loadCubemap(vector<const GLchar*> faces);

//全局变量区
const GLuint WIDTH = 800, HEIGHT = 600;
GLuint creatFramebuffer();
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool    keys[1024];
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glewExperimental = GL_TRUE;
	glewInit();

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	glEnable(GL_DEPTH);

	

	//----creat shader---------------------------------
	Shader worldShader = Shader("world.vs", "world.frag");
	Shader skyShader = Shader("sky.vs", "sky.frag");
	Shader reflectShader = Shader("cubereflect.vs", "cubereflect.frag");

	//load texture-----------------------------------
	GLuint texMap = loadTexture("texMap.jpg", false);
	//load texture of skybox 
	vector<const GLchar*> faces;
	faces.push_back("skybox/right.jpg");
	faces.push_back("skybox/left.jpg");
	faces.push_back("skybox/top.jpg");
	faces.push_back("skybox/bottom.jpg");
	faces.push_back("skybox/back.jpg");
	faces.push_back("skybox/front.jpg");
	GLuint cubemapTexture = loadCubemap(faces);



	//set VAO、VBO of cube------------------/
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

	GLuint VBO, cubeVAO;  //创建两个句柄VBO、VAO
	glGenVertexArrays(1, &cubeVAO);//把VAO的固有ID给句柄
	glGenBuffers(1, &VBO);//把VBO的固有ID给另一个句柄					  
	glBindVertexArray(cubeVAO);//将VAO绑定到顶点数组
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //将VBO绑定到顶点缓冲区
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);//把内存中的顶点数组传到显存中的顶点缓冲区（里面有顶点缓冲对象VBO）
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);//设置顶点属性指针、顶点着色器按照该指针方式从顶点缓冲区中提取数据到自己的顶点属性中
	glEnableVertexAttribArray(0);//使用该顶点属性指针
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);//使用该顶点属性指针
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);//使用该顶点属性指针
	glBindBuffer(GL_ARRAY_BUFFER, 0);// 解绑顶点缓冲区的VBO
	glBindVertexArray(0); // 解绑VAO

	
    //set skyboxVAO、skyboxVBO
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	GLuint skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	

	//set framebuff of six director
	GLuint framebuffer[6];
	for (int i = 0; i < 6; i++)
	{
		framebuffer[i] = creatFramebuffer();
	}


	//set camera of six director
	glm::vec3 cubeUp[6] = 
	{
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	};
	
	glm::vec3 cubeFront[6]=
	{
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, -1.0f)
	};
	glm::mat4 cubeView[6];
	for (int index = 0; index < 6; index++)
	{
		cubeView[index] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), cubeFront[index], cubeUp[index]);
	}


	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		GLfloat nowtime = glfwGetTime();
		deltaTime = nowtime - lastFrame;
		lastFrame = nowtime;
		glfwPollEvents();
		do_movement();

		//-----------------------render in framebuffer------------------------------////////

		for (int i = 0; i < 6; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[i]);
			glEnable(GL_DEPTH_TEST);
			// Clear the colorbuffer
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//render 
			worldShader.Use();
			glm::mat4 view;
			view = cubeView[i];//此时是从金属盒位置看向大理石盒子
			glm::mat4 projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
			GLint modelLoc = glGetUniformLocation(worldShader.Program, "model");
			GLint viewLoc = glGetUniformLocation(worldShader.Program, "view");
			GLint projLoc = glGetUniformLocation(worldShader.Program, "projection");
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
			glEnable(GL_CULL_FACE);
			
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D,texMap);
			glUniform1i(glGetUniformLocation(worldShader.Program, "ourTexture"), 0);

			glBindVertexArray(cubeVAO);
			glm::mat4 modelBox = glm::translate(modelBox, glm::vec3(8.0f, 0.0f, 0.0f));//大理石盒子向右移动一点
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelBox));
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
			//关闭面剔除
			glDisable(GL_CULL_FACE);

			//再渲染天空盒
			//渲染天空盒
			glDepthFunc(GL_LEQUAL);//让深度测试判断设置为小于等于
			skyShader.Use();
			GLint skyViewLoc = glGetUniformLocation(skyShader.Program, "view");
			glm::mat4 skyView = cubeView[i];
			glUniformMatrix4fv(skyViewLoc, 1, GL_FALSE, glm::value_ptr(skyView));

			glm::mat4 skyProjection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
			GLint skyProjLoc = glGetUniformLocation(skyShader.Program, "projection");
			glUniformMatrix4fv(skyProjLoc, 1, GL_FALSE, glm::value_ptr(skyProjection));

			glBindVertexArray(skyboxVAO);
			glActiveTexture(GL_TEXTURE0);
			glUniform1i(glGetUniformLocation(skyShader.Program, "skybox"), 0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
			glDepthFunc(GL_LESS);//让深度测试判断该为小于
	      }


		//将6个帧缓冲组装成立方体贴图
		GLuint dynamicMap;//动态环境贴图
		glGenTextures(1, &dynamicMap);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, dynamicMap);
		for (GLuint i = 0; i < faces.size(); i++)
		{
			unsigned char* image = NULL;
			glReadPixels(0.0, 0.0, HEIGHT, WIDTH, GL_RGB,GL_UNSIGNED_BYTE,image);//从帧缓冲中读取颜色数据
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
				GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
			);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	



		//开始在默认缓冲中渲染屏幕，在这个屏幕渲染天空盒以及金属立方体///////////////////////
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//先渲染金属盒
		glm::mat4 view;
		view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(camera.Zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(reflectShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(reflectShader.Program, "view");
		GLint projLoc = glGetUniformLocation(reflectShader.Program, "projection");
		glm::mat4 model;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		//传摄像机位置
		glUniform3f(glGetUniformLocation(reflectShader.Program, "cameraPos"),camera.Position.x, camera.Position.y, camera.Position.z);
		//传环境（环境光反射）
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(glGetUniformLocation(reflectShader.Program, "skybox"), 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, dynamicMap);
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		//再渲染天空盒
		glDepthFunc(GL_LEQUAL);//让深度测试判断设置为小于等于
		skyShader.Use();
		GLint skyViewLoc = glGetUniformLocation(skyShader.Program, "view");
		glm::mat4 skyView = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		glUniformMatrix4fv(skyViewLoc, 1, GL_FALSE, glm::value_ptr(skyView));

		glm::mat4 skyProjection = glm::perspective(camera.Zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
		GLint skyProjLoc = glGetUniformLocation(skyShader.Program, "projection");
		glUniformMatrix4fv(skyProjLoc, 1, GL_FALSE, glm::value_ptr(skyProjection));

		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(glGetUniformLocation(skyShader.Program, "skybox"), 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);//让深度测试判断该为小于
		
		glfwSwapBuffers(window);
	}
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &skyboxVBO);

	//Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}


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

GLuint creatFramebuffer()
{
	GLuint framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	//创建一个纹理对象，长宽设置为屏幕宽度
	GLuint texColorBuffer;
	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	// 将该纹理对象绑定到帧缓冲颜色附件上
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);
	//创建一个渲染缓冲对象
	GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT);//设定该渲染缓冲中24位用于深度缓冲、8位用于模板缓冲
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	//把渲染缓冲对象附加到帧缓冲的深度和模板附件上
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	//检查帧缓冲对象是否创建成功
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return framebuffer;
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//放大用邻近过滤为了能看清像素点
																	 // Load image, create texture and generate mipmaps
	int txture_width, txture_height;
	unsigned char* image = SOIL_load_image(path, &txture_width, &txture_height, 0, alpha ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, alpha ? GL_RGBA : GL_RGB, txture_width, txture_height, 0, alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	return texture;
}

GLuint loadCubemap(vector<const GLchar*> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);

	int width, height;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (GLuint i = 0; i < faces.size(); i++)
	{
		image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
			GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
		);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}
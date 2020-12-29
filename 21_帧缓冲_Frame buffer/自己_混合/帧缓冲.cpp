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
GLuint loadTexture(GLchar* path, GLboolean alpha);//加载纹理函数,参数1：路径；参数2：是否有alpha值
const GLuint WIDTH = 800, HEIGHT = 600;
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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//设置帧缓冲，添加纹理附件和渲染缓冲附件//////////////////
	//创建一个帧缓冲对象，并绑定它，帧缓冲有颜色附件（一般使用纹理）、深度附件、模板附件（一般使用渲染缓冲）
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

	// 创建shader//////////////////////////////////////////////
	Shader worldProgram = Shader("world.vs", "world.frag");
	Shader screenShader = Shader("screen.vs", "screen.frag");


	//设置地板的顶点数组和相应的VAO,VBO///////////////////////
	float floor_vertices[] = {
		// Positions               // Texture Coords
		-0.5f, -0.5f, -0.5f,  0.0f, 5.0f,
		0.5f, -0.5f, -0.5f,  5.0f, 5.0f,
		0.5f, -0.5f,  0.5f,  5.0f, 0.0f,

		0.5f, -0.5f,  0.5f,  5.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 5.0f

	};
	GLuint floorVBO, floorVAO;  //创建两个句柄VBO、VAO
	glGenVertexArrays(1, &floorVAO);//把VAO的固有ID给句柄
	glGenBuffers(1, &floorVBO);//把VBO的固有ID给另一个句柄
						  // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(floorVAO);//将VAO绑定到顶点数组
	glBindBuffer(GL_ARRAY_BUFFER, floorVBO); //将VBO绑定到顶点缓冲区
	glBufferData(GL_ARRAY_BUFFER, sizeof(floor_vertices), floor_vertices, GL_STATIC_DRAW);//把内存中的顶点数组传到显存中的顶点缓冲区（里面有顶点缓冲对象VBO）
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);//设置顶点属性指针、顶点着色器按照该指针方式从顶点缓冲区中提取数据到自己的顶点属性中
	glEnableVertexAttribArray(0);//使用该顶点属性指针
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);//使用该顶点属性指针
	glBindBuffer(GL_ARRAY_BUFFER, 0);// 解绑顶点缓冲区的VBO
	glBindVertexArray(0); // 解绑VAO


	

//设置立方体的VAO.VBO/////////////
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
	GLuint objectVBO, objectVAO;  //创建两个句柄VBO、VAO
	glGenVertexArrays(1, &objectVAO);//把VAO的固有ID给句柄
	glGenBuffers(1, &objectVBO);//把VBO的固有ID给另一个句柄
							   // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(objectVAO);//将VAO绑定到顶点数组
	glBindBuffer(GL_ARRAY_BUFFER, objectVBO); //将VBO绑定到顶点缓冲区
	glBufferData(GL_ARRAY_BUFFER, sizeof(object_vertices), object_vertices, GL_STATIC_DRAW);//把内存中的顶点数组传到显存中的顶点缓冲区（里面有顶点缓冲对象VBO）
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);//设置顶点属性指针、顶点着色器按照该指针方式从顶点缓冲区中提取数据到自己的顶点属性中
	glEnableVertexAttribArray(0);//使用该顶点属性指针
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);//使用该顶点属性指针
	glBindBuffer(GL_ARRAY_BUFFER, 0);// 解绑顶点缓冲区的VBO
	glBindVertexArray(0); 

	//设置小草的顶点数组、VAO和VBO///////////
	float grass_vertices[] = {
		0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
		0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

		0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
		1.0f,  0.5f,  0.0f,  1.0f,  0.0f
	};
	GLuint grassVBO, grassVAO;  //创建两个句柄VBO、VAO
	glGenVertexArrays(1, &grassVAO);//把VAO的固有ID给句柄
	glGenBuffers(1, &grassVBO);//把VBO的固有ID给另一个句柄
								// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(grassVAO);//将VAO绑定到顶点数组
	glBindBuffer(GL_ARRAY_BUFFER, grassVBO); //将VBO绑定到顶点缓冲区
	glBufferData(GL_ARRAY_BUFFER, sizeof(grass_vertices), grass_vertices, GL_STATIC_DRAW);//把内存中的顶点数组传到显存中的顶点缓冲区（里面有顶点缓冲对象VBO）
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);//设置顶点属性指针、顶点着色器按照该指针方式从顶点缓冲区中提取数据到自己的顶点属性中
	glEnableVertexAttribArray(0);//使用该顶点属性指针
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);//使用该顶点属性指针
	glBindBuffer(GL_ARRAY_BUFFER, 0);// 解绑顶点缓冲区的VBO
	glBindVertexArray(0);

	//设置屏幕的顶点数组、VAO和VBO///////////
	float screen_vertices[] = {
		1.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-1.0f, 1.0f,  0.0f,  0.0f,  1.0f,

		-1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		-1.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		1.0f,  -1.0f,  0.0f,  1.0f,  0.0f
	};
	GLuint screenVBO, screenVAO;  //创建两个句柄VBO、VAO
	glGenVertexArrays(1, &screenVAO);//把VAO的固有ID给句柄
	glGenBuffers(1, &screenVBO);//把VBO的固有ID给另一个句柄
							   // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(screenVAO);//将VAO绑定到顶点数组
	glBindBuffer(GL_ARRAY_BUFFER, screenVBO); //将VBO绑定到顶点缓冲区
	glBufferData(GL_ARRAY_BUFFER, sizeof(screen_vertices), screen_vertices, GL_STATIC_DRAW);//把内存中的顶点数组传到显存中的顶点缓冲区（里面有顶点缓冲对象VBO）
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);//设置顶点属性指针、顶点着色器按照该指针方式从顶点缓冲区中提取数据到自己的顶点属性中
	glEnableVertexAttribArray(0);//使用该顶点属性指针
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);//使用该顶点属性指针
	glBindBuffer(GL_ARRAY_BUFFER, 0);// 解绑顶点缓冲区的VBO
	glBindVertexArray(0);

  //设置物体的纹理/////////////////////////////////////////////
	GLuint cube_texture = loadTexture("2.jpg",false);//立方体纹理	
	GLuint floor_texture = loadTexture("3.jpg",false);//地面纹理
	GLuint grass_texture = loadTexture("grass.png", true);//草的纹理带alpha值

	

	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		GLfloat nowtime = glfwGetTime();
		deltaTime = nowtime - lastFrame;
		lastFrame = nowtime;
		glfwPollEvents();
		do_movement();

		//开始在帧缓冲中渲染整个场景//////////////////////
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glEnable(GL_DEPTH_TEST);
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

	


		//渲染地板
		//激活地板纹理单元
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, floor_texture);
		glUniform1i(glGetUniformLocation(worldProgram.Program, "ourTexture"), 0);
		glBindVertexArray(floorVAO);
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, -0.01f, 0.0f));//让地板向下平移一点，避免深度冲突
		model = glm::scale(model, glm::vec3(5.0f, 1.0f, 5.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		//渲染立方体
		//开启面剔除
		glEnable(GL_CULL_FACE);
		//激活物体纹理单元
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cube_texture);
		glUniform1i(glGetUniformLocation(worldProgram.Program, "ourTexture"), 0);
		glBindVertexArray(objectVAO);
		glm::mat4 model1;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model1));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		//关闭面剔除
		glDisable(GL_CULL_FACE);

		//渲染小草
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, grass_texture);
		glUniform1i(glGetUniformLocation(worldProgram.Program, "ourTexture"), 0);
		glBindVertexArray(grassVAO);
		glm::mat4 model2;
		model2 = glm::scale(model2, glm::vec3(0.5f, 0.5f, 1.0f));
		model2 = glm::translate(model2, glm::vec3(0.0f, -0.5f, 1.0f));//让地板向下平移一点，避免深度冲突
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		//开始在默认缓冲中渲染屏幕，在这个屏幕上贴上帧缓冲中的纹理附件///////////////////////
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		screenShader.Use();
		glBindTexture(GL_TEXTURE_2D, texColorBuffer);
		glUniform1i(glGetUniformLocation(screenShader.Program, "screenTexture"), 0);
		glBindVertexArray(screenVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);



		glfwSwapBuffers(window);
	}
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &objectVAO);
	glDeleteVertexArrays(1, &floorVAO);
	glDeleteVertexArrays(1, &grassVAO);
	glDeleteVertexArrays(1, &screenVAO);
	glDeleteBuffers(1, &floorVBO);
	glDeleteBuffers(1, &objectVBO);
	glDeleteBuffers(1, &grassVBO);
	glDeleteBuffers(1, &screenVBO);
	//Terminate GLFW, clearing any resources allocated by GLFW.
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//放大用邻近过滤为了能看清像素点
																	 // Load image, create texture and generate mipmaps
	int txture_width, txture_height;
	unsigned char* image = SOIL_load_image(path, &txture_width, &txture_height, 0, alpha? SOIL_LOAD_RGBA: SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, alpha ? GL_RGBA : GL_RGB, txture_width, txture_height, 0, alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	return texture;
}


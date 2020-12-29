#define GLEW_STATIC
#include <iostream>
#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/SOIL.h>
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glew32s.lib") 
#pragma comment (lib, "glfw3.lib")
#pragma comment (lib, "SOIL.lib")
#define GLM_FORCE_RADIANS  //Bug

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Window dimensions
const GLuint WIDTH = 1000, HEIGHT = 600;
GLfloat MIXTION = 1.0f;

// Shaders
const GLchar* vertexShaderSource = "#version 330 core\n"  //顶点着色器代码
    "layout (location = 0) in vec3 position;\n"
	"layout (location = 1) in vec3 color;\n" 
	"layout (location = 2) in vec2 texCoord;\n"
	"uniform mat4 transform;\n"
	"out vec3 ourColor;\n"
	"out vec2 TexCoord;\n"

	"void main(){\n"
	"gl_Position = transform * vec4(position.x,position.y,position.z,1.0f);\n"
	"ourColor = color;\n"
	"TexCoord = vec2(texCoord.s,1.0-texCoord.t);\n"
    "}\0";
const GLchar* fragmentShaderSource = "#version 330 core\n"//片段着色器代码
	"in vec3 ourColor;\n"
	"in vec2 TexCoord;\n"
    "out vec4 color;\n"
	"uniform sampler2D ourTexture1;\n"
	"uniform sampler2D ourTexture2;\n"
	"uniform float mixtion;\n"
    "void main()\n"
    "{\n"
	"color = mix(texture(ourTexture1,TexCoord) ,texture(ourTexture2,TexCoord), mixtion);\n"
	//"color = texture(ourTexture1,TexCoord);\n"
    "}\n\0";

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

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();

    // Define the viewport dimensions
    glViewport(0, 0, WIDTH, HEIGHT);


    // Build and compile our shader program
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);  //创建一个顶点着色器
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);//给该顶点着色器加上代码
    glCompileShader(vertexShader); //测试是否成功
    // Check for compile time errors
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);  //创建一个片段着色器
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);  //给该判断着色器加上代码
    glCompileShader(fragmentShader);//测试是否成功
    // Check for compile time errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Link shaders
    GLuint shaderProgram = glCreateProgram();  //创建一个着色器程序（管线）
    glAttachShader(shaderProgram, vertexShader); //将顶点着色器加入着色器程序
    glAttachShader(shaderProgram, fragmentShader);//将片段着色器加入着色器程序
    glLinkProgram(shaderProgram);//链接所有着色器
    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);//测试是否成功
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);//删除两个着色器
    glDeleteShader(fragmentShader);


    // Set up vertex data (and buffer(s)) and attribute pointers
    GLfloat vertices[] = {
        // Positions          // Colors           // Texture Coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // Top Right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // Top Left 
    };
    GLuint indices[] = {  // Note that we start from 0!
        0, 1, 3, // First Triangle
        1, 2, 3  // Second Triangle
    };
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    // TexCoord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0); // Unbind VAO


    // Load and create a texture 
    GLuint texture1,texture2;
	/////////////////////////
    glGenTextures(1, &texture1);
	  glBindTexture(GL_TEXTURE_2D, texture1);
    // Set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//放大用邻近过滤为了能看清像素点
    // Load image, create texture and generate mipmaps
    int width, height;
    unsigned char* image = SOIL_load_image("1.jpg", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
	///////////////////////////////////////////////////////////////////////////////////////
  glGenTextures(1, &texture2);
 glBindTexture(GL_TEXTURE_2D, texture2);
    // Set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load image, create texture and generate mipmaps
    image = SOIL_load_image("awesomeface.png", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.





    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();

        // Render
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        // Activate shader
        glUseProgram(shaderProgram);

		    glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture1"), 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture2"), 1);
    
		GLint mixLocation = glGetUniformLocation(shaderProgram,"mixtion");
		glUniform1f(mixLocation,MIXTION);

	    glm::mat4 transform;
        transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
        transform = glm::rotate(transform, (GLfloat)glfwGetTime() * 50.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram,"transform"), 1, GL_FALSE, glm::value_ptr(transform));
		

        // Draw container
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

		glm::mat4 transform1; 
	    transform1 = glm::translate(transform1, glm::vec3(-0.5f, 0.5f, 0.0f));
		transform1 = glm::scale(transform1, glm::vec3(sin((float)glfwGetTime()),sin((float)glfwGetTime()),0.0));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram,"transform"), 1, GL_FALSE, glm::value_ptr(transform1));
		

        // Draw container
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Swap the screen buffers
        glfwSwapBuffers(window);
    }
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		MIXTION += 0.1f;
		if(MIXTION >= 1.0f)
			MIXTION = 1.0f;
	}

	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		MIXTION -= 0.1f;
		if( MIXTION <= 0.0f)
			MIXTION = 0.0f;
	}

}
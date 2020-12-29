#version 330 core //顶点着色器代码
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;//法向量
layout (location = 2) in vec2 texCoords;
out vec3 Normal;//法向量
out vec3 FragPos;//片段位置
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;



    void main()
    {
    gl_Position = projection * view * model * vec4(position, 1.0f);
	Normal = Normal = mat3(transpose(inverse(model))) * normal ; //当立方体进行不规则缩放时，规范化法向量（法向量乘以一个正规矩阵）
	FragPos = vec3(model * vec4(position,1.0f)) ;
	TexCoords = texCoords;
    }
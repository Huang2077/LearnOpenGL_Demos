#version 330 core //顶点着色器代码
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;//法向量
layout (location = 2) in vec2 texCoords;
out VS_OUT
{
 vec3 Normal;
 vec3 FragPos;
 vec2 TexCoords;
}vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;



    void main()
    {
    gl_Position = projection * view * model * vec4(position, 1.0f);
	vs_out.Normal  = mat3(transpose(inverse(model))) * normal ; //当立方体进行不规则缩放时，规范化法向量（法向量乘以一个正规矩阵）
	vs_out.FragPos = vec3(model * vec4(position,1.0f)) ;
	vs_out.TexCoords = texCoords;
    }
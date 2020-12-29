#version 330 core
layout (location = 0) in vec3 position;
out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	vec4 Pos =   projection * view * vec4(position, 1.0);  
	gl_Position = Pos.xyww;//使用前置深度测试
    TexCoords = position;//立方体贴图纹理坐标就等于片段位置坐标（立方体以原点为中心）
}
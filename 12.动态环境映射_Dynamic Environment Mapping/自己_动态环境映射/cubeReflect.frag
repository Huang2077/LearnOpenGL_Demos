#version 330 core//片段着色器代码

in vec3 Normal;//法向量
in vec3 FragPos;//片段位置
in vec2 TexCoords;
out vec4 color;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
    {
	vec3 norm = normalize(Normal);
	//模型全部反射天空盒的颜色
	vec3 I = normalize( FragPos - cameraPos);//片段与玩家视角所成角度
	vec3 R = reflect(I,norm);
    vec3 reflect_color = vec3( texture(skybox, R)) * 0.7;
	color = vec4(reflect_color, 1.0f);  	
    }
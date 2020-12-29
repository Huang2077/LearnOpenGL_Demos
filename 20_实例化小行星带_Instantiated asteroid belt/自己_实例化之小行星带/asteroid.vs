#version 330 core

layout (location = 0) in vec3 Atr_Position;
layout (location = 2) in vec2 Atr_Texcoord;
layout (location = 3) in mat4 instanceMatrix;
out vec2 Vs_out_Texcoord;

uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * instanceMatrix * vec4(Atr_Position, 1.0f);
	Vs_out_Texcoord = Atr_Texcoord;
}

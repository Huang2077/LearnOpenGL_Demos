#version 330 core
in vec2 Vs_out_Texcoord;

out vec4 Fg_out_Color;

uniform sampler2D texture_diffuse1;

void main()
{
	Fg_out_Color = texture(texture_diffuse1, Vs_out_Texcoord);
}


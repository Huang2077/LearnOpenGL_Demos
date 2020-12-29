#version 330 core

in vec3 gs_out_color;
out vec4 fg_out_color;
void main()
{
	fg_out_color = vec4(gs_out_color,1.0);
}
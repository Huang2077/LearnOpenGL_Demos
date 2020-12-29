

#version 330 core

layout (location = 0) in vec3 Atr_Position;
layout (location = 1) in vec3 Atr_v0;
layout (location = 2) in float Atr_inittime;

uniform vec3 a;
uniform float now_time;


vec3 GetOffset()
{
 float t = now_time - Atr_inittime;
 return (Atr_v0 * t + 0.5 * a * t * t);
}

void main()
{
 vec3 offset = GetOffset();
 vec3 New_Position = Atr_Position + offset;
 gl_Position =  vec4(New_Position, 1.0);
 gl_PointSize = 8.0;
}






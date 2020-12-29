#version 330 core//Æ¬¶Î×ÅÉ«Æ÷´úÂë
in vec3 TexCoords;

out vec4 color;

uniform samplerCube skybox;

void main()
{
 color = texture(skybox, TexCoords);
}
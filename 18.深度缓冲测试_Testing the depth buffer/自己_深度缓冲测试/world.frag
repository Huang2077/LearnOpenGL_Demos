#version 330 core//Ƭ����ɫ������


in vec2 TexCoords;
out vec4 color;
uniform sampler2D ourTexture;

void main()
    {
	//color = vec4(vec3(gl_FragCoord.z),1.0f); //Ƭ�ε����ֵ
    color = texture(ourTexture,TexCoords);
    }
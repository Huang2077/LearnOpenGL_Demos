#version 330 core//Ƭ����ɫ������

in vec3 Normal;//������
in vec3 FragPos;//Ƭ��λ��
in vec2 TexCoords;
out vec4 color;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
    {
	vec3 norm = normalize(Normal);
	//ģ��ȫ��������պе���ɫ
	vec3 I = normalize( FragPos - cameraPos);//Ƭ��������ӽ����ɽǶ�
	vec3 R = reflect(I,norm);
    vec3 reflect_color = vec3( texture(skybox, R)) * 0.7;
	color = vec4(reflect_color, 1.0f);  	
    }
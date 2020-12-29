#version 330 core
layout (location = 0) in vec3 position;
out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	vec4 Pos =   projection * view * vec4(position, 1.0);  
	gl_Position = Pos.xyww;//ʹ��ǰ����Ȳ���
    TexCoords = position;//��������ͼ��������͵���Ƭ��λ�����꣨��������ԭ��Ϊ���ģ�
}
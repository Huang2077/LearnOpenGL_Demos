#version 330 core//片段着色器代码


in vec2 TexCoords;
out vec4 color;
uniform sampler2D ourTexture;

void main()
    {
	//color = vec4(vec3(gl_FragCoord.z),1.0f); //片段的深度值
    color = texture(ourTexture,TexCoords);
    }
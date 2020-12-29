#version 330 core
layout(points) in;
layout(triangle_strip, max_vertices = 5) out;

in VS_OUT{
	vec3 color;
}gs_in[];

out vec3 gs_out_color;

void HouseBuild(vec4 position)
{   
    gs_out_color = gs_in[0].color; // gs_in[0] since there's only one input vertex
    gl_Position = position + vec4(-0.2f, -0.2f, 0.0f, 0.0f);    // 1:bottom-left   
    EmitVertex();   
    gl_Position = position + vec4( 0.2f, -0.2f, 0.0f, 0.0f);    // 2:bottom-right
    EmitVertex();
    gl_Position = position + vec4(-0.2f,  0.2f, 0.0f, 0.0f);    // 3:top-left
    EmitVertex();
    gl_Position = position + vec4( 0.2f,  0.2f, 0.0f, 0.0f);    // 4:top-right
    EmitVertex();
    gl_Position = position + vec4( 0.0f,  0.4f, 0.0f, 0.0f);    // 5:top
    gs_out_color = vec3(1.0f, 1.0f, 1.0f);
    EmitVertex();
    EndPrimitive();
	
}
void main(){
	HouseBuild(gl_in[0].gl_Position);
}

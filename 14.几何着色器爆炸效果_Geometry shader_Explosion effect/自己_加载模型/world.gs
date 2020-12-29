#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT{
	vec3 Normal;
	vec3 FragPos;
	vec2 TexCoords;
}gs_in[];

out GS_OUT{
	vec3 Normal;
	vec3 FragPos;
	vec2 TexCoords;
}gs_out;
uniform float time;

vec3 GetNormal( )
{
	vec3 a = vec3(gl_in[0].gl_Position - gl_in[1].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position - gl_in[1].gl_Position);
	return normalize(cross(a, b));
}

vec4 Explode(vec4 Position, vec3 Normal)
{
	float magnitude = 2.0f;
    vec3 direction = Normal * ((sin(time) + 1.0f) / 2.0f) * magnitude;
    return Position + vec4(direction, 0.0f);
}

void main()
{
	vec3 normal = GetNormal();

	gl_Position = Explode(gl_in[0].gl_Position, normal);
	gs_out.Normal = gs_in[0].Normal;
	gs_out.FragPos = gs_in[0].FragPos;
	gs_out.TexCoords = gs_in[0].TexCoords;
	EmitVertex();

	gl_Position = Explode(gl_in[1].gl_Position, normal);
	gs_out.Normal = gs_in[1].Normal;
	gs_out.FragPos = gs_in[1].FragPos;
	gs_out.TexCoords = gs_in[1].TexCoords;
	EmitVertex();

	gl_Position = Explode(gl_in[2].gl_Position, normal);
	gs_out.Normal = gs_in[2].Normal;
	gs_out.FragPos = gs_in[2].FragPos;
	gs_out.TexCoords = gs_in[2].TexCoords;
	EmitVertex();

	EndPrimitive();
}
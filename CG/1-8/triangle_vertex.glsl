#version 330 core

//out vec3 outColor;

uniform vec2 uPos;

void main()
{
	// gl_VertexID

	vec4 outPos;

	float size = 0.1f;

	if (gl_VertexID == 0){
		outPos = vec4(0.0, 2 * size, 0.0, 1.0);
		//outColor = vec3(1.f, 0.f, 0.f);
	}
	else if (gl_VertexID == 1){
		outPos = vec4(-size, -size, 0.0, 1.0);
		//outColor = vec3(0.f, 1.f, 0.f);
	}
	else if (gl_VertexID == 2){
		outPos = vec4(size, -size, 0.0, 1.0);
		//outColor = vec3(0.f, 0.f, 1.f);
	}

	gl_Position = outPos + vec4(uPos, 0.f, 0.f);
}
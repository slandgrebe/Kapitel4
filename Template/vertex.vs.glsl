#version 430 core \n
\n
void main(void) { \n
	// Declare a hard-coded array of positions \n
	const vec4 vertices[3] = vec4[3]	(vec4( 0.25, -0.25, 0.5, 1.0), \n
										vec4(-0.25, -0.25, 0.5, 1.0), \n
										vec4( 0.25,  0.25, 0.5, 1.0)); \n
	
	// Add offset to our hard-coded vertex position \n
	gl_Position = vertices[gl_VertexID]; \n
} \n
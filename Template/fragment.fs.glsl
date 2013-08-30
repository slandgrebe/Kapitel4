#version 430 core

// Output to the framebuffer
out vec4 color;

void main(void) {
	// Simply assign the color we were given by the vertex shader
	// to our output
	color = vec4(0.0, 0.8, 1.0, 1.0);
}
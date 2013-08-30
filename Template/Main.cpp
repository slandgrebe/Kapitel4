// Include the "sb6.h" header file
#include "sb6.h"
#include "math.h"
#include "shader.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

// Derive Test from sb6::application
class Kapitel4 : public sb6::application {
private:
	GLuint renderingProgram;
	GLuint vertexArrayObject;

public:
	void init() {
		static const char title[] = "OpenGL Math";
		sb6::application::init();
        memcpy(info.title, title, sizeof(title));
	}

	// Our rendering function
	void render(double currentTime) {
		const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, green);

		// Use the program object we created earlier for rendering
		glUseProgram(renderingProgram);

		GLfloat attrib[] = { 0.0f, 0.0f, 0.0f, 0.0f };

		// Update the value of input attribute 0
		glVertexAttrib4fv(0, attrib);

		// color
		GLfloat triangleColor[] = {	1.0f, 1.0f,	1.0f, 1.0f };
		glVertexAttrib4fv(1, triangleColor);

		// Draw one triangle
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	GLchar const * loadShaderSource(const string filePath) {
		ifstream in("file.txt");
		stringstream buffer;
		buffer << in.rdbuf();
		string source = buffer.str();

		cout << "hallo";
		cout << source << endl << endl;
		system("pause");

		return source.c_str();
	}

	GLuint compileShaders(void) {

		/*if (renderingProgram)
			glDeleteProgram(renderingProgram);

		GLuint shaders[5];

		shaders[0] = sb6::shader::load("vertex.vs.glsl", GL_VERTEX_SHADER);
		shaders[1] = sb6::shader::load("tessallationControl.tcs.glsl", GL_TESS_CONTROL_SHADER);
		shaders[2] = sb6::shader::load("tessellationEvaluation.tes.glsl", GL_TESS_EVALUATION_SHADER);
		shaders[3] = sb6::shader::load("geometry.gs.glsl", GL_GEOMETRY_SHADER);
		shaders[4] = sb6::shader::load("fragment.fs.glsl", GL_FRAGMENT_SHADER);

		GLuint program = sb6::program::link_from_shaders(shaders, 5, true);
		
		return program;*/

		GLuint vertexShader;
		GLuint tcShader;
		GLuint teShader;
		GLuint geometryShader;
		GLuint fragmentShader;
		GLuint program;

		// Source code for vertex shader
		static const GLchar* vertexShaderSource[] = {
			"#version 430 core \n"
			"\n"
			"void main(void) { \n"
			"	// Declare a hard-coded array of positions \n"
			"	const vec4 vertices[3] = vec4[3]	(vec4( 0.25, -0.25, 0.5, 1.0), \n"
			"										 vec4(-0.25, -0.25, 0.5, 1.0), \n"
			"										 vec4( 0.25,  0.25, 0.5, 1.0)); \n"
			" \n"
			"	// Add offset to our hard-coded vertex position \n"
			"	gl_Position = vertices[gl_VertexID]; \n"
			"} \n"
		};
		//static const GLchar* vertexShaderSource[] = { loadShaderSource("vertex.shader") };

		// Source code for the tessallation control shader
		static const GLchar* tcsSource[] = {
			"#version 430 core \n"
			" \n"
			"layout (vertices = 3) out; \n"
			" \n"
			"void main(void) { \n"
			"	if (gl_InvocationID == 0) { \n"
			"		gl_TessLevelInner[0] = 5.0; \n"
			"		gl_TessLevelOuter[0] = 5.0; \n"
			"		gl_TessLevelOuter[1] = 5.0; \n"
			"		gl_TessLevelOuter[2] = 5.0; \n"
			"	} \n"
			"	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position; \n"
			"} \n"
		};
		
		// Source code for tessallation evaluation shader
		static const GLchar* tesSource[] = {
			"#version 430 core \n"
			" \n"
			"layout (triangles, equal_spacing, cw) in; \n"
			" \n"
			"void main(void) { \n"
			"	gl_Position =  (gl_TessCoord.x * gl_in[0].gl_Position + \n"
			"					gl_TessCoord.y * gl_in[1].gl_Position + \n"
			"					gl_TessCoord.z * gl_in[2].gl_Position); \n"
			"} \n"
		};

		// Source code for geometry shader
		static const GLchar* geometryShaderSource[] = {
			"#version 430 core \n"
			" \n"
			"layout (triangles) in; \n"
			"layout (points, max_vertices = 3) out; \n"
			" \n"
			"void main(void) { \n"
			"	int i; \n"
			"	for (i = 0; i < gl_in.length(); i++) { \n"
			"		gl_Position = gl_in[i].gl_Position; \n"
			"		EmitVertex(); \n"
			"	} \n"
			"} \n"
		};

		// Source code for fragment shader
		static const GLchar* fragmentShaderSource[] = {
			"#version 430 core \n"
			" \n"
			"// Output to the framebuffer \n"
			"out vec4 color; \n"
			" \n"
			"void main(void) { \n"
			"	// Simply assign the color we were given by the vertex shader \n"
			"	// to our output \n"
			"	color = vec4(0.0, 0.8, 1.0, 1.0); \n"
			"} \n"
		};

		// Create and compile vertex shader
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, vertexShaderSource, NULL);
		glCompileShader(vertexShader);

		// Create and compile tessallation control shader
		tcShader = glCreateShader(GL_TESS_CONTROL_SHADER);
		glShaderSource(tcShader, 1, tcsSource, NULL);
		glCompileShader(tcShader);

		// Create and compile tessallation evaluation shader
		teShader = glCreateShader(GL_TESS_EVALUATION_SHADER);
		glShaderSource(teShader, 1, tesSource, NULL);
		glCompileShader(teShader);

		// Create and compile geometry shader
		geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometryShader, 1, geometryShaderSource, NULL);
		glCompileShader(geometryShader);
		
		// Create and compile fragment shader
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);

		// Create program, attach shader to it, and link it
		program = glCreateProgram();
		glAttachShader(program, vertexShader);
		//glAttachShader(program, tcShader);
		//glAttachShader(program, teShader);
		//glAttachShader(program, geometryShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);

		// Delete the shaders as the program has them now
		glDeleteShader(vertexShader);
		glDeleteShader(tcShader);
		glDeleteShader(teShader);
		glDeleteShader(geometryShader);
		glDeleteShader(fragmentShader);
		
		return program;
	}

	void startup() {
		renderingProgram = compileShaders();
		glGenVertexArrays(1, &vertexArrayObject);
		glBindVertexArray(vertexArrayObject);

		// Only draw the outlines of the triangles, to see the result of the tessallation
		glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLES);
		//glPointSize(5.0f);
	}
	
	void shutdown() {
		glDeleteVertexArrays(1, &vertexArrayObject);
		glDeleteProgram(renderingProgram);
		//glDeleteVertexArrays(1, &vertexArrayObject);
	}
};

// Our on and only instance of DECLARE_MAIN
DECLARE_MAIN(Kapitel4);
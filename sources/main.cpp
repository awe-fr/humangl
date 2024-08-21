#include "./../includes/header.h"

float deltaTime;

static const GLfloat g_vertex_buffer_data[] = {
	0.5f, 0.5f, 0.5f,
   -0.5f, 0.5f, -0.5f,
    -0.5f, 0.5f, 0.5f,
    0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,
    0.5f, -0.5f, 0.5f, 
    -0.5f, -0.5f, 0.5f
};

unsigned int Indices[] = { // Top triangles
                               0, 1, 2,
                              1, 3, 4,
                              5, 6, 3,
                              7, 3, 6,
                              2, 4, 7,
                              0, 7, 6,
                              0, 5, 1,
                              1, 5, 3,
                              5, 0, 6,
                              7, 4, 3,
                              2, 1, 4,
                              0, 2, 7 };

void getDeltaTime(void) {
	static double last = glfwGetTime();

	double now = glfwGetTime();
	deltaTime = (float)(now - last);
	last = now;
}

int main(void) {
	WindowsApp *app = new WindowsApp();

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint IBO;
	glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

	GLuint programID = LoadShaders( "./shaders/shader.vert", "./shaders/shader.frag" );
	
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	float ref = 0;

	while(app->isClosed() != true) {
		getDeltaTime();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(programID);

		vec3 eye; eye.x = 4; eye.y = 3; eye.z = 3;
		vec3 target; target.x = 0; target.y = 0; target.z = 0;
		vec3 up; up.x = 0; up.y = 1; up.z = 0;

		mat4 view = movement(app->getWindow());

		mat4 translation = translationMat(0, 0, 0);

		mat4 rotation = rotationMatY(ref);
		ref += 0.005;

		mat4 Projection = projectionMat(120.0f, (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
		
		mat4 MVP = matMult(Projection, matMult(view, matMult(translation, rotation)));

		float temp[16];
		populateMat(temp, MVP);

		GLuint projection = glGetUniformLocation(programID, "MVP");
		glUniformMatrix4fv(projection, 1, GL_FALSE, temp);

 



		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glDisableVertexAttribArray(0);
	}
	delete app;
	return (0);
}

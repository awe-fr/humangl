#include "./../includes/header.h"

static const GLfloat g_vertex_buffer_data[] = {
   -1.0f, -1.0f, 0.0f,
   1.0f, -1.0f, 0.0f,
   0.0f,  1.0f, 0.0f,
};

void printmat(mat4 print) {
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			std::cout << print.data[y][x] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
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

	GLuint programID = LoadShaders( "./shaders/shader.vert", "./shaders/shader.frag" );
	
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	while(app->isClosed() != true) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(programID);

		mat4 projection = projectionMat(90, WIDTH / HEIGHT, 0.1f, 100.0f);

		vec3 eye; eye.x = 4; eye.y = 3; eye.z = 3;
		vec3 target; target.x = 0; target.y = 0; target.z = 0;
		vec3 up; up.x = 0; up.y = 1; up.z = 0;
		mat4 view = viewMat(eye, target, up);

		mat4 model = identityMat(1.0f);

		GLuint ProjectionID = glGetUniformLocation(programID, "Projection");
		GLuint ViewID = glGetUniformLocation(programID, "View");
		GLuint ModelID = glGetUniformLocation(programID, "Model");

		float projectionBuf[16] = {0};
		float viewBuf[16] = {0};
		float modelBuf[16] = {0};

		populateMat(projectionBuf, projection);
		populateMat(viewBuf, view);
		populateMat(modelBuf, model);

		glUniformMatrix4fv(ProjectionID, 1, GL_FALSE, projectionBuf);
		glUniformMatrix4fv(ViewID, 1, GL_FALSE, viewBuf);
		glUniformMatrix4fv(ModelID, 1, GL_FALSE, modelBuf);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDisableVertexAttribArray(0);
	}
	delete app;
	return (0);
}

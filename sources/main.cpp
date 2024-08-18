#include "./../includes/header.h"

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

void printmat(mat4 print) {
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			std::cout << print.data[y][x] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

float getRad(float toRad) {
	return (toRad * (M_PI / 2));
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

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);

	float set = 0;
	while(app->isClosed() != true) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(programID);

		mat4 translation = identityMat(1);

		translation.data[2][3] = 1;

		mat4 rotation = identityMat(1);

		rotation.data[0][0] = cos(set);
		rotation.data[0][2] = -sin(set);
		rotation.data[2][0] = sin(set);
		rotation.data[2][2] = cos(set);

		set += 0.0005;

		// mat4 Projection = identityMat(1);

		// float FOV = 90;
    	// float const tanHalfFovy = tan(getRad(FOV / 2));
		// float p = 1/ tanHalfFovy;

		// Projection.data[0][0] = p;
		// Projection.data[1][1] = p;
		// Projection.data[3][2] = 1;
		// Projection.data[3][3] = 0;
		float FOV = 90.0f; // ou un autre angle qui vous convient
		float aspectRatio = WIDTH / HEIGHT;
		float nearPlane = 0.1f;
		float farPlane = 100.0f;
		float tanHalfFovy = tan(FOV / 2.0f);

		mat4 Projection = identityMat(1.0f); // Assuming you have an identity matrix function

		Projection.data[0][0] = 1.0f / (aspectRatio * tanHalfFovy);
		Projection.data[1][1] = 1.0f / tanHalfFovy;
		Projection.data[2][2] = farPlane / (farPlane - nearPlane);
		Projection.data[2][3] = 1.0f;  // Notice this is positive, for left-handed coordinates
		Projection.data[3][2] = -(nearPlane * farPlane) / (farPlane - nearPlane);
		Projection.data[3][3] = 0.0f;

		
		Projection = matMult(Projection, matMult(translation, rotation));


		GLuint projection = glGetUniformLocation(programID, "Projection");
		glUniformMatrix4fv(projection, 1, GL_TRUE, &Projection.data[0][0]);


		// mat4 projection = projectionMat(90, WIDTH / HEIGHT, 0.1f, 100.0f);

		// vec3 eye; eye.x = 4; eye.y = 3; eye.z = 3;
		// vec3 target; target.x = 0; target.y = 0; target.z = 0;
		// vec3 up; up.x = 0; up.y = 1; up.z = 0;
		// mat4 view = viewMat(eye, target, up);

		// mat4 model = identityMat(1.0f);

		// GLuint ProjectionID = glGetUniformLocation(programID, "Projection");
		// GLuint ViewID = glGetUniformLocation(programID, "View");
		// GLuint ModelID = glGetUniformLocation(programID, "Model");

		// float projectionBuf[16] = {0};
		// float viewBuf[16] = {0};
		// float modelBuf[16] = {0};

		// populateMat(projectionBuf, projection);
		// populateMat(viewBuf, view);
		// populateMat(modelBuf, model);

		// glUniformMatrix4fv(ProjectionID, 1, GL_TRUE, projectionBuf);
		// glUniformMatrix4fv(ViewID, 1, GL_FALSE, viewBuf);
		// glUniformMatrix4fv(ModelID, 1, GL_FALSE, modelBuf);


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

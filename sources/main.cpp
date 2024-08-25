#include "./../includes/WindowApp.hpp"
#include "./../includes/Member.hpp"

int main(void) {
	WindowsApp *app = new WindowsApp();

	Member *test1 = new Member("test", 1);

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);


	float ref1 = 180;
	float ref2 = 35;
	float ref3 = 287;
	float ref4 = 90;

	while(app->isClosed() != true) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(app->getProgramID());

		glBindVertexArray(test1->getVAO());
		glBindBuffer(GL_ARRAY_BUFFER, test1->getVBO());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, test1->getIBO());

		app->computeMovement();

		mat4 model = matMult(rotationMatZ(ref1), translationMat(0, 0.0f, 0));
		ref1 += 0.055;
		ref2 -= 0.010;
		ref3 -= 0.040;
		ref4 += 0.070;
		
		mat4 MVP = matMult(app->getProjection(), matMult(app->getView(), model));

		float temp[16];
		populateMat(temp, MVP);

		GLuint mvp = glGetUniformLocation(app->getProgramID(), "MVP");
		glUniformMatrix4fv(mvp, 1, GL_FALSE, temp);


		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glDisableVertexAttribArray(0);
	}
	delete test1;
	delete app;
	return (0);
}

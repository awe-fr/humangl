#include "./../includes/WindowApp.hpp"
#include "./../includes/Member.hpp"

int main(void) {
	WindowsApp *app = new WindowsApp();

	Member *test1 = new Member("test1", 1);
	Member *test2 = new Member("test2", 1);
	Member *test3 = new Member("test3", 1);
	
	test2->setPrevious(test1);
	test3->setPrevious(test2);

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	float ref1 = 180;
	float ref2 = 35;
	float ref3 = 287;
	float ref4 = 90;

	float mvpPopulated[16];

	MemberList *inst = MemberList::getInstance();
	std::vector<Member *> lst = inst->getList();
	while(app->isClosed() != true) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		app->computeMovement();
		glUseProgram(app->getProgramID());

		test1->setDegree(ref3, ref1, ref4);
		test2->setDegree(45, 0, 60);
		test3->setDegree(45, 60, 0);

		ref3+= 0.005;
		ref1+= 0.005;
		ref4+= 0.005;

		test1->computeTravel();
		test2->computeTravel();
		test3->computeTravel();

		for (int i = 0; i < lst.size(); i++) {
			glBindVertexArray(lst[i]->getVAO());
			glBindBuffer(GL_ARRAY_BUFFER, lst[i]->getVBO());
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lst[i]->getIBO());

			mat4 MVP = matMult(app->getProjection(), matMult(app->getView(), lst[i]->getModel()));

			populateMat(mvpPopulated, MVP);

			GLuint mvp = glGetUniformLocation(app->getProgramID(), "MVP");
			glUniformMatrix4fv(mvp, 1, GL_FALSE, mvpPopulated);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			glDisableVertexAttribArray(0);
		}
	}
	inst->cleanup();
	inst->deleteInstance();
	delete app;
	return (0);
}

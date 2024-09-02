#include "./../includes/WindowApp.hpp"
#include "./../includes/Member.hpp"
#include "./../includes/InputParser.hpp"

int main(void) {
	InputParser input_parser("animations/asf/02.asf", "animations/amc/walk.amc");
	input_parser.parseASF();

	WindowsApp *app = new WindowsApp();

	Member *test1 = new Member("test1", 1);
	Member *test2 = new Member("test2", 1);
	Member *test3 = new Member("test3", 1);

	vec4 color; color.x = 0.33; color.y = 0.33; color.z = 0.33; color.w = 1;
	
	test2->setPrevious(test1);
	test3->setPrevious(test2);

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	float ref1 = 0;
	float ref2 = 0;
	float ref3 = 0;
	float ref4 = 0;

	float mvpPopulated[16];
	float vec4Populated[16];
	populateVec4(vec4Populated, color);

	MemberList *inst = MemberList::getInstance();
	std::vector<Member *> lst = inst->getList();
	while(app->isClosed() != true) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		app->computeMovement();
		glUseProgram(app->getProgramID());

		test1->setDegree(ref1, ref2, ref3);
		test2->setDegree(45, 0, 60);
		test3->setDegree(45, 60, 0);

		test1->computeTravel();
		test2->computeTravel();
		test3->computeTravel();

		for (int i = 0; i < lst.size(); i++) {
			mat4 MVP = matMult(app->getProjection(), matMult(app->getView(), lst[i]->getModel()));
			populateMat(mvpPopulated, MVP);
			GLuint mvp = glGetUniformLocation(app->getProgramID(), "MVP");
			glUniformMatrix4fv(mvp, 1, GL_FALSE, mvpPopulated);
			
			GLuint colorvec = glGetUniformLocation(app->getProgramID(), "cvec");
			glUniform4fv(colorvec, 1, vec4Populated);


			glBindVertexArray(lst[i]->getVAO());

			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, lst[i]->getVBO());
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lst[i]->getIBO());
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			glDisableVertexAttribArray(0);
		}

		ImGui::SetNextWindowSize(ImVec2(250, 125), ImGuiCond_FirstUseEver);
		ImGui::Begin("HumanGL settings");

		ImGui::SliderFloat("X", &ref1, -360, 360);
		ImGui::SliderFloat("Y", &ref2, -360, 360);
		ImGui::SliderFloat("Z", &ref3, -360, 360);

		ImGui::ColorEdit3("color", vec4Populated);

		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	inst->cleanup();
	inst->deleteInstance();
	delete app;
	return (0);
}

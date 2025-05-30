#include "../includes/WindowApp.hpp"
#include "../includes/Member.hpp"
#include "../includes/InputParser.hpp"
#include "../includes/Singleton.hpp"
#include "../includes/Animation.hpp"
#include "../includes/ImguiValues.hpp"

int main(void) {
	InputParser input_parser("animations/asf/02.asf", "animations/amc/walk.amc");
	input_parser.parseASF();
	input_parser.parseAMC();
	
	WindowsApp *app = new WindowsApp();
	input_parser.buildMembers();

	vec4 color; color.x = 0.33; color.y = 0.33; color.z = 0.33; color.w = 1;

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	float mvpPopulated[16];
	float vec4Populated[16];
	populateVec4(vec4Populated, color);

	MemberList *inst = MemberList::getInstance();
	std::vector<Member *> lst = inst->getList();

	ImguiValues imgui_values;

	for (std::vector<Member *>::iterator it = lst.begin(); it != lst.end(); it++)
		(*it)->attach(&imgui_values);

	// for (int i = 0; i < lst.size(); i++) {
	// 	lst[i]->printName();
	// }

	Animation *walk = input_parser.getAnimation();
	walk->setRoot(input_parser.getRoot());

	while(app->isClosed() != true) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		app->computeMovement();
		glUseProgram(app->getProgramID());

		imgui_values.notify();

		// walk->play();

		for (std::vector<Member *>::iterator it = lst.begin(); it != lst.end(); it++)
			(*it)->computeTravel();

		Root *test = input_parser.getRoot();
		mat4 MVP = matMult(app->getProjection(), matMult(app->getView(), test->getModel()));
		populateMat(mvpPopulated, MVP);
		GLuint mvp = glGetUniformLocation(app->getProgramID(), "MVP");
		glUniformMatrix4fv(mvp, 1, GL_FALSE, mvpPopulated);
		
		GLuint colorvec = glGetUniformLocation(app->getProgramID(), "cvec");
		glUniform4fv(colorvec, 1, vec4Populated);

		glBindVertexArray(test->getVAO());

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, test->getVBO());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, test->getIBO());
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glDisableVertexAttribArray(0);

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

		ImGui::SliderFloat("Length", imgui_values.getLength(), 0, 5);
		ImGui::SliderFloat("Width", imgui_values.getWidth(), 0, 5);
		ImGui::SliderFloat("Head size", imgui_values.getHeadSize(), 0, 5);

		ImGui::ColorEdit3("color", vec4Populated);

		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	for (std::vector<Member *>::iterator it = lst.begin(); it != lst.end(); it++)
		(*it)->detach();

	inst->cleanup();
	inst->deleteInstance();
	delete app;
	return (0);
}

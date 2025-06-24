#include "../includes/BVHAnimation.hpp"
#include "../includes/WindowApp.hpp"
#include "../includes/Singleton.hpp"
#include "../includes/ImguiValues.hpp"
#include "../includes/manageAnim.hpp"

int main(int argc, char **argv)
{
	if (argc != 1)
	{
		std::cout << "Invalid arguments.\nUsage: ./humangl" << std::endl;
		exit(0);
	}
	
	WindowsApp *app = new WindowsApp();

	vec4 color; color.x = 0.33; color.y = 0.33; color.z = 0.33; color.w = 1;

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	float mvpPopulated[16];
	float vec4Populated[16];
	populateVec4(vec4Populated, color);

	ImguiValues imgui_values;

	manageAnim *manager = new manageAnim(&imgui_values);
	if (manager->setupAnimation() == true)
		return 1;
	manager->attach(&imgui_values);

	BVHAnimation *animation = imgui_values.getAnim();

	MemberList *inst = MemberList::getInstance();

	std::vector<BVHAnimation::Member *> lst = inst->getList();

	const char* labels[9] = {
		"Walk", "Run", "Jump",
		"Climb", "Punch", "Sword",
		"Dance 1", "Dance 2", "Dance 3"
	};
	
	while(app->isClosed() != true) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	
		animation = imgui_values.getAnim();
		inst = MemberList::getInstance();
		lst = inst->getList();
		
		app->computeMovement();
		glUseProgram(app->getProgramID());

		imgui_values.notify();

		animation->Run();

		for (std::vector<BVHAnimation::Member *>::iterator it = lst.begin(); it != lst.end(); it++)
			(*it)->computeTravel();
		
		for (std::vector<BVHAnimation::Member *>::iterator it = lst.begin(); it != lst.end(); it++)
			(*it)->computeTravel2();

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

		ImGui::SetNextWindowSize(ImVec2(250, 260), ImGuiCond_FirstUseEver);
		ImGui::Begin("HumanGL settings");

		ImGui::SliderFloat("Length", imgui_values.getLength(), 0, 5);
		ImGui::SliderFloat("Width", imgui_values.getWidth(), 0, 5);
		ImGui::SliderFloat("Head size", imgui_values.getHeadSize(), 0, 5);

		ImGui::ColorEdit3("color", vec4Populated);

		ImGui::Text("");
		ImGui::Text("Animations :");

		for (int i = 0; i < 9; ++i) {
			if (ImGui::Button(labels[i], ImVec2(60, 30))) {
				imgui_values.setAnimName(labels[i]);
			}
			if ((i + 1) % 3 != 0)
				ImGui::SameLine();
		}

		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	}

	manager->detachAnimation();
	manager->detach();

	inst->cleanup();
	inst->deleteInstance();
	delete manager;
	delete app;
	return (0);
}
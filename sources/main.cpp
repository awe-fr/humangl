#include "../includes/WindowApp.hpp"
#include "../includes/Member.hpp"
#include "../includes/InputParser.hpp"
#include "../includes/Singleton.hpp"
#include "../includes/Animation.hpp"

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

	for (int i = 0; i < lst.size(); i++) {
		lst[i]->printName();
	}

	size_t nb_frames = input_parser.getAnimation()->getNumberFrames();
	std::vector<Frame> frames = input_parser.getAnimation()->getFrames();
	size_t i = 0;

	while(app->isClosed() != true) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		app->computeMovement();
		glUseProgram(app->getProgramID());

		// if (i == nb_frames)
		// 	i = 0;
		// Frame frame = frames[i];
		// for (std::map<std::string, std::vector<float>>::iterator it = frame.bones.begin(); it != frame.bones.end(); it++)
		// {
		// 	if (it->first == ASF_KEY_ROOT)
		// 	{
		// 		Root *root = input_parser.getRoot();
		// 		vec3 position;
		// 		vec3 orientation;
		// 		for (size_t j = 0; j < it->second.size(); j++)
		// 		{
		// 			switch (j)
		// 			{
		// 				case 0:
		// 					position.x = it->second[j];
		// 					break;

		// 				case 1:
		// 					position.y = it->second[j];
		// 					break;

		// 				case 2:
		// 					position.z = it->second[j];
		// 					break;
						
		// 				case 3:
		// 					orientation.x = it->second[j];
		// 					break;

		// 				case 4:
		// 					orientation.y = it->second[j];
		// 					break;

		// 				case 5:
		// 					orientation.z = it->second[j];
		// 					break;

		// 				default:
		// 					break;
		// 			}
		// 		}

		// 		root->setPosition(position);
		// 		root->setOrientation(orientation);
		// 	}
		// 	else
		// 	{
		// 		Member *member = inst->findByName(it->first);
		// 		std::map<std::string, Limit> degree_lock = member->getDegreeLock();
		// 		size_t j = 0;

		// 		for (std::map<std::string, Limit>::iterator itt = degree_lock.begin(); itt != degree_lock.end(); itt++)
		// 		{
		// 			if (itt->first == "rx")
		// 				member->setDegreeX(it->second[j]);
		// 			else if (itt->first == "ry")
		// 				member->setDegreeY(it->second[j]);
		// 			else if (itt->first == "rz")
		// 				member->setDegreeZ(it->second[j]);
		// 			j++;
		// 		}
		// 	}
		// }

		// i++;

		for (std::vector<Member *>::iterator it = lst.begin(); it != lst.end(); it++)
			(*it)->computeTravel();

		// Root *test = input_parser.getRoot();
		// mat4 MVP = matMult(app->getProjection(), matMult(app->getView(), test->getModel()));
		// populateMat(mvpPopulated, MVP);
		// GLuint mvp = glGetUniformLocation(app->getProgramID(), "MVP");
		// glUniformMatrix4fv(mvp, 1, GL_FALSE, mvpPopulated);
		
		// GLuint colorvec = glGetUniformLocation(app->getProgramID(), "cvec");
		// glUniform4fv(colorvec, 1, vec4Populated);

		// glBindVertexArray(test->getVAO());

		// glEnableVertexAttribArray(0);
		// glBindBuffer(GL_ARRAY_BUFFER, test->getVBO());
		// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, test->getIBO());
		// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

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

		ImGui::SetNextWindowSize(ImVec2(250, 55), ImGuiCond_FirstUseEver);
		ImGui::Begin("HumanGL settings");

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

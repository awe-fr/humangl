#include "../includes/manageAnim.hpp"

manageAnim::manageAnim(ImguiValues *imgui_values) : _imgui_values(imgui_values) {

}

manageAnim::~manageAnim() {
	AnimationList *instAnim = AnimationList::getInstance();

	std::vector<BVHAnimation *> lst = instAnim->getList();

	for (int i = 0; i < lst.size(); i++) {
		delete lst[i];
	}

	instAnim->cleanup();
	instAnim->deleteInstance();
}

bool	manageAnim::setupAnimation() {
	BVHAnimation *climb = new BVHAnimation("./animations/bvh/swordplay.bvh", "Climb");
	BVHAnimation *dance1 = new BVHAnimation("./animations/bvh/dance1.bvh", "Dance 1");
	BVHAnimation *dance2 = new BVHAnimation("./animations/bvh/dance2.bvh", "Dance 2");
	BVHAnimation *dance3 = new BVHAnimation("./animations/bvh/dance3.bvh", "Dance 3");
	BVHAnimation *dance4 = new BVHAnimation("./animations/bvh/dance4.bvh", "Dance 4");
	BVHAnimation *jump = new BVHAnimation("./animations/bvh/jump.bvh", "Jump");
	BVHAnimation *punch = new BVHAnimation("./animations/bvh/punch.bvh", "Punch");
	BVHAnimation *run = new BVHAnimation("./animations/bvh/run.bvh", "Run");
	BVHAnimation *swordplay = new BVHAnimation("./animations/bvh/swordplay.bvh", "Sword");
	BVHAnimation *walk = new BVHAnimation("./animations/bvh/walk.bvh", "Walk");
	try
	{
		climb->Parse();
		dance1->Parse();
		dance2->Parse();
		dance3->Parse();
		dance4->Parse();
		jump->Parse();
		punch->Parse();
		run->Parse();
		swordplay->Parse();
		walk->Parse();
	}
	catch(BVHAnimation::Exception& e)
	{
		std::cerr << e.what();
        return true;
	}

	AnimationList *instAnim = AnimationList::getInstance();
	instAnim->add(climb);
	instAnim->add(dance1);
	instAnim->add(dance2);
	instAnim->add(dance3);
	instAnim->add(dance4);
	instAnim->add(jump);
	instAnim->add(punch);
	instAnim->add(run);
	instAnim->add(swordplay);
	instAnim->add(walk);

	this->_imgui_values->setAnim(walk);

	std::vector<BVHAnimation::Member *> members = walk->GetMembers();
	MemberList *instMember = MemberList::getInstance();
	for (int i = 0; i < members.size(); i++) {
		instMember->add(members[i]);
		members[i]->attach(this->_imgui_values);
	}
    return false;
}

void	manageAnim::detachAnimation() {
	BVHAnimation *animation = this->_imgui_values->getAnim();

	std::vector<BVHAnimation::Member *> members = animation->GetMembers();

	for (int i = 0; i < members.size(); i++) {
		members[i]->detach();
	}
}

void	manageAnim::update(void *param) {
	Param *ratio = (Param *)param;

	switch (ratio->type)
	{
		case Anim_name:
		{
			AnimationList *instAnim = AnimationList::getInstance();
			BVHAnimation *animation = instAnim->findByName(ratio->name);

			if (animation) {
				std::vector<BVHAnimation::Member *> members = animation->GetMembers();
				MemberList *instMember = MemberList::getInstance();
				instMember->cleanup();
				detachAnimation();
				for (int i = 0; i < members.size(); i++) {
					instMember->add(members[i]);
					members[i]->attach(this->_imgui_values); 
					this->_imgui_values->setAnim(animation);
				}
			}
			break;
		}
				
		default:
			break;
	}
}
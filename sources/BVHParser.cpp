#include "../includes/BVHParser.hpp"

BVHParser::BVHParser(std::string path) : _bvhPath(path), _root(nullptr) {
	MemberList *inst = MemberList::getInstance();

	this->_root = new Root({"Xposition", "Yposition", "Zposition", "Zrotation", "Yrotation", "Xrotation"});

	Member *lhipjoint = new Member("lhipjoint", {0.655637, -0.713449, 0.247245}, 2.52691, {0, 0, 0}, nullptr, this->_root);
	Member *lfemur = new Member("lfemur", {0.34202, -0.939693, 0}, 7.59371, {0, 0, 0}, lhipjoint, nullptr);
	Member *ltibia = new Member("ltibia", {0.34202, -0.939693, 0}, 7.28717, {0, 0, 0}, lfemur, nullptr);
	Member *lfoot = new Member("lfoot", {0.0886837, -0.243657, 0.965798}, 2.2218, {0, 0, 0}, ltibia, nullptr);
	Member *ltoes = new Member("ltoes", {0, 0, 1}, 1.11249, {0, 0, 0}, lfoot, nullptr);

	Member *rhipjoint = new Member("rhipjoint", {-0.645062, -0.722004, 0.250209}, 2.49697, {0, 0, 0}, nullptr, this->_root);
	Member *rfemur = new Member("rfemur", {-0.34202, -0.939693, 0}, 7.58734, {0, 0, 0}, rhipjoint, nullptr);
	Member *rtibia = new Member("rtibia", {-0.34202, -0.939693, 0}, 7.21538, {0, 0, 0}, rfemur, nullptr);
	Member *rfoot = new Member("rfoot", {-0.102907, -0.282735, 0.953662}, 2.23735, {0, 0, 0}, rtibia, nullptr);
	Member *rtoes = new Member("rtoes", {0, 0, 1}, 1.11569, {0, 0, 0}, rfoot, nullptr);

	Member *lowerback = new Member("lowerback", {0.00952301, 0.997604, -0.0685217}, 2.05943, {0, 0, 0}, nullptr, this->_root);
	Member *upperback = new Member("upperback", {0.00494399, 0.999577, -0.0286701}, 2.06523, {0, 0, 0}, lowerback, nullptr);
	Member *thorax = new Member("thorax", {0.000729686, 0.999992, 0.00388603}, 2.06807, {0, 0, 0}, upperback, nullptr);
	Member *lowerneck = new Member("lowerneck", {0.00452882, 0.995459, 0.0950816}, 1.57426, {0, 0, 0}, thorax, nullptr);
	Member *upperneck = new Member("upperneck", {0.00452882, 0.995459, 0.0950816}, 1.57426, {0, 0, 0}, lowerneck, nullptr);
	Member *head = new Member("head", {0.008024, 0.999444, -0.0323691}, 1.6265, {0, 0, 0}, upperneck, nullptr);

	Member *lclavicle = new Member("lclavicle", {0.967826, 0.247107, -0.0474446}, 3.6598, {0, 0, 0}, thorax, nullptr);
	Member *lhumerus = new Member("lhumerus", {1, 0, 0}, 4.86513, {0, 0, 0}, lclavicle, nullptr);
	Member *lradius = new Member("lradius", {1, 0, 0}, 3.35554, {0, 0, 0}, lhumerus, nullptr);
	Member *lwrist = new Member("lwrist", {1, 0, 0}, 1.67777, {0, 0, 0}, lradius, nullptr);
	Member *lhand = new Member("lhand", {1, 0, 0}, 0.661175, {0, 0, 0}, lwrist, nullptr);
	Member *lfingers = new Member("lfingers", {1, 0, 0}, 0.533057, {0, 0, 0}, lhand, nullptr);
	Member *lthumb = new Member("lthumb", {1, 0, 0}, 0.765366, {0, 0, 0}, lfingers, nullptr);

	Member *rclavicle = new Member("rclavicle", {-0.973174, 0.211421, -0.0907389}, 3.59444, {0, 0, 0}, thorax, nullptr);
	Member *rhumerus = new Member("rhumerus", {-1, 0, 0}, 5.02649, {0, 0, 0}, rclavicle, nullptr);
	Member *rradius = new Member("rradius", {-1, 0, 0}, 3.36431, {0, 0, 0}, rhumerus, nullptr);
	Member *rwrist = new Member("rwrist", {-1, 0, 0}, 1.68216, {0, 0, 0}, rradius, nullptr);
	Member *rhand = new Member("rhand", {-1, 0, 0}, 0.730406, {0, 0, 0}, rwrist, nullptr);
	Member *rfingers = new Member("rfingers", {-1, 0, 0}, 0.588872, {0, 0, 0}, rhand, nullptr);
	Member *rthumb = new Member("rthumb", {-1, 0, 0}, 0.845506, {0, 0, 0}, rfingers, nullptr);

	inst->add(lhipjoint);
	inst->add(lfemur);
	inst->add(ltibia);
	inst->add(lfoot);
	inst->add(ltoes);

	inst->add(rhipjoint);
	inst->add(rfemur);
	inst->add(rtibia);
	inst->add(rfoot);
	inst->add(rtoes);

	inst->add(lowerback);
	inst->add(upperback);
	inst->add(thorax);
	inst->add(lowerneck);
	inst->add(upperneck);
	inst->add(head);

	inst->add(lclavicle);
	inst->add(lhumerus);
	inst->add(lradius);
	inst->add(lwrist);
	inst->add(lhand);
	inst->add(lfingers);
	inst->add(lthumb);

	inst->add(rclavicle);
	inst->add(rhumerus);
	inst->add(rradius);
	inst->add(rwrist);
	inst->add(rhand);
	inst->add(rfingers);
	inst->add(rthumb);
}

BVHParser::~BVHParser() {}

// static void trim(std::string &line)
// {
// 	size_t i = 0;

// 	while (i != line.size() && (line[i] == ' ' || line[i] == '\t' || line[i] == '\n' || line[i] == '\r'))
// 		i++;

// 	line = line.substr(i);
// }

static std::vector<std::string> split(std::string line)
{
	std::vector<std::string> ret;
	std::string tmp;

	size_t i = 0;

	while (i != line.size()) {
		while (i != line.size() && (line[i] == ' ' || line[i] == '\t' || line[i] == '\n' || line[i] == '\r'))
			i++;
		while (i != line.size() && (line[i] != ' ' && line[i] != '\t' && line[i] != '\n' && line[i] != '\r')) {
			tmp += line[i];
			i++;
		}
		if (tmp.length() > 0) {
			ret.push_back(tmp);
			tmp = "";
		}
	}

	return (ret);
}

bool	BVHParser::parseVBH() {
	this->_bvhFile.open(this->_bvhPath);

	if (!this->_bvhFile) {
		std::cerr << "BVH input file error: file '" << this->_bvhPath << "' does not exist." << std::endl;
		return false;
	}

	std::string line;

	while (std::getline(this->_bvhFile, line)) {
		std::vector<std::string> splitted = split(line);

		if (splitted.size() == 0) {
			continue;
		}

		if (splitted[0] == "HIERARCHY") {
			std::cout << splitted[0] << std::endl;
			return (parseHierarchy());
		} else if (splitted[0] == "MOTION") {
			
		}
	}
	std::cout << "28" << std::endl;
	return false;
}

bool	BVHParser::parseHierarchy() {
	std::string line;

	while (std::getline(this->_bvhFile, line)) {
		std::vector<std::string> splitted = split(line);

		if (splitted.size() == 0) {
			continue;
		}

		if (splitted[0] == "ROOT") {
			std::cout << splitted[0] << " " << splitted[1] << std::endl;
			if (splitted.size() != 2) {
				std::cout << "25" << std::endl;
				return false;
			}
			return parseRoot(splitted[1]);
		} else {
			std::cout << "26" << std::endl;
			return false;
		}
	}
	std::cout << "27" << std::endl;
	return false;
}

bool	BVHParser::parseRoot(std::string name) {
	std::string line;
	std::vector<int> offest;
	std::vector<std::string> channels;
	std::string next;
	std::vector<std::string> splitted;

	while (std::getline(this->_bvhFile, line)) {
		splitted = split(line);

		if (splitted.size() == 0) {
			continue;
		}
		if (splitted[0] == "{") {
			std::cout << splitted[0] << std::endl;
			if (splitted.size() != 1) {
				std::cout << "24" << std::endl;
				return false;
			}
			if (offest.size() == 0 || channels.size() == 0 || next.length() == 0) {
				while(std::getline(this->_bvhFile, line) && (offest.size() == 0 || channels.size() == 0 || next.length() == 0)) {
					splitted = split(line);
					if (splitted.size() == 0) {
						continue;
					}
					if (splitted[0] == "OFFSET") {
						if (offest.size() != 0 || splitted.size() != 4) {
							std::cout << "23" << std::endl;
							return false;
						}
						std::cout << splitted[0] << " " << splitted[1] << " " << splitted[2] << " " << splitted[3] << std::endl;
						for (int i = 1; i <= 3; i++) {
							offest.push_back(stoi(splitted[i]));
						}
					} else if (splitted[0] == "CHANNELS") {
						if (channels.size() != 0 || splitted.size() != 8) {
							std::cout << "22" << std::endl;
							return false;
						}
						if (splitted[1] != "6" || splitted[2] != "Xposition" || splitted[3] != "Yposition" || splitted[4] != "Zposition" || splitted[5] != "Zrotation" || splitted[6] != "Yrotation" || splitted[7] != "Xrotation") {
							std::cout << "21" << std::endl;
							return false;
						}
						for (int i = 2; i <= 7; i++) {
							channels.push_back(splitted[i]);
						}
						std::cout << splitted[2] << " " << splitted[3] << " " << splitted[4] << " " << splitted[5] << " " << splitted[6] << " " << splitted[7] << std::endl;
					} else if (splitted[0] == "JOINT") {
						if (next.length() != 0 || splitted.size() != 2) {
							std::cout << "20" << std::endl;
							return false;
						}
						next = splitted[1];
						std::cout << splitted[1] << std::endl;
					} else if ((splitted[0] == "}" || splitted[0] == "{")) {
						std::cout << "19" << std::endl;
						return false;
					}
				}
			}
			splitted = split(line);
			if (splitted.size() == 0) {
				continue;
			}
			if (splitted[0] == "{" && this->_root == nullptr) {
				std::cout << splitted[0] << std::endl;
				this->_root = new Root(channels);
				if (parseMember(next) == false) {
					std::cout << "18" << std::endl;
					return false;
				}
			} else if (splitted[0] == "}" && this->_root == nullptr) {
				this->_root = new Root(channels);
				std::cout << "root created" << std::endl;
				return true;
			} else if (splitted[0] == "{") {
				std::cout << splitted[0] << std::endl;
				if (parseMember(next) == false) {
					std::cout << "17" << std::endl;
					return false;
				}
			} else if (splitted[0] == "}") {
				return true;
			}
			else {
				std::cout << "16" << std::endl;
				return false;
			}
		} else if (splitted[0] == "JOINT" && offest.size() != 0 && channels.size() != 0 && next.length() != 0 && splitted.size() == 2) {
			std::cout << splitted[1] << " tests" << std::endl;
			next = splitted[1];
		} else if (splitted[0] == "{" && this->_root != nullptr) {
			if (parseMember(next) == false) {
				std::cout << "31" << std::endl;
				return false;
			}
		} else if (splitted[0] == "}" && this->_root != nullptr) {
			return true;
		} else {
			std::cout << "15" << std::endl;
			return false;
		}
	}
	std::cout << "14" << std::endl;
	return false;
}

bool	BVHParser::parseMember(std::string name) {
	std::string line;
	std::vector<int> offest;
	std::vector<std::string> channels;
	std::string next;
	std::vector<std::string> splitted;
	bool endSite = false;

	while(std::getline(this->_bvhFile, line) && (offest.size() == 0 || channels.size() == 0 || (next.length() == 0 && endSite == false))) {
		splitted = split(line);

		if (splitted.size() == 0) {
			continue;
		}

		if (splitted[0] == "OFFSET") {
			if (offest.size() != 0 || splitted.size() != 4) {
				std::cout << "12" << std::endl;
				return false;
			}
			std::cout << splitted[0] << " " << splitted[1] << " " << splitted[2] << " " << splitted[3] << std::endl;
			for (int i = 1; i <= 3; i++) {
				offest.push_back(stoi(splitted[i]));
			}
		} else if (splitted[0] == "CHANNELS") {
			if (channels.size() != 0 || splitted.size() != 5) {
				std::cout << "11" << std::endl;
				return false;
			}
			if (splitted[1] != "3" || splitted[2] != "Zrotation" || splitted[3] != "Yrotation" || splitted[4] != "Xrotation") {
				std::cout << "10" << std::endl;
				return false;
			}
			for (int i = 2; i <= 4; i++) {
				channels.push_back(splitted[i]);
			}
			std::cout << splitted[2] << " " << splitted[3] << " " << splitted[4] << std::endl;
		} else if (splitted[0] == "JOINT") {
			if (next.length() != 0 || splitted.size() != 2) {
				std::cout << "9" << std::endl;
				return false;
			}
			next = splitted[1];
			std::cout << splitted[1] << std::endl;
		} else if (splitted[0] == "End" && splitted[1] == "Site" && next.length() == 0) {
			endSite = true;
			std::cout << splitted[0] << std::endl;
			if (parseEndSite() == false) {
				std::cout << "8" << std::endl;
				return false;
			}
		} else if ((splitted[0] == "}" || splitted[0] == "{")) {
			std::cout << "7" << std::endl;
			return false;
		}
	}
	splitted = split(line);
	
	if (splitted.size() != 0) {
		if (splitted[0] == "{") {
			// cree le membre
			std::cout << splitted[0] << std::endl;
			if (parseMember(next) == false) {
				std::cout << "4" << std::endl;
				return false;
			}
		} else if (splitted[0] == "}" && offest.size() != 0 && channels.size() != 0 && next.length() == 0 && endSite == true) {
			std::cout << splitted[0] << std::endl;
			return true;
		} else if (splitted[0] == "}" && (offest.size() == 0 || channels.size() == 0 || next.length() == 0)) {
			std::cout << "5" << std::endl;
			return false;
		} else {
			std::cout << "6" << std::endl;
			return false;
			// std::cout << line << " esfe " << name << std::endl;
		}
	}
	while(std::getline(this->_bvhFile, line)) {
		splitted = split(line);

		if (splitted.size() == 0) {
			continue;
		}
		if (splitted[0] == "{") {
			// cree le membre
			if (parseMember(next) == false) {
				std::cout << "0" << std::endl;
				return false;
			}
		} else if (splitted[0] == "}" && offest.size() != 0 && channels.size() != 0 && next.length() == 0 && endSite == true) {
			std::cout << splitted[0] << std::endl;
			return true;
		} else if (splitted[0] == "}" && (offest.size() == 0 || channels.size() == 0 || next.length() == 0)) {
			std::cout << "1" << std::endl;
			return false;
		} else if (splitted[0] == "}") {
			std::cout << splitted[0] << std::endl;
			return true;
		} else if (splitted[0] == "JOINT" && offest.size() != 0 && channels.size() != 0 && next.length() != 0 && splitted.size() == 2) {
			std::cout << splitted[1] << std::endl;
			return true;
		} else {
			std::cout << "2" << std::endl;
			return false;
		}
	}

	std::cout << "3" << std::endl;
	return false;

}

bool	BVHParser::parseEndSite() {
	std::string line;
	std::vector<std::string> splitted;

	while(std::getline(this->_bvhFile, line)) {
		splitted = split(line);
		std::cout << splitted[0] << std::endl;
		if (splitted[0] == "}")
			return true;
	}
	std::cout << "13" << std::endl;
	return false;
}
#include "../includes/BVHParser.hpp"

BVHParser::BVHParser(std::string path) : _bvhPath(path), _root(nullptr) {}

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
			if (splitted.size() != 2)
				return false;
			return parseRoot(splitted[1]);
		} else {
			return false;
		}
	}

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
			if (splitted.size() != 1)
				return false;
			if (offest.size() == 0 || channels.size() == 0 || next.length() == 0) {
				while(std::getline(this->_bvhFile, line) && (offest.size() == 0 || channels.size() == 0 || next.length() == 0)) {
					splitted = split(line);
					if (splitted.size() == 0) {
						continue;
					}
					if (splitted[0] == "OFFSET") {
						if (offest.size() != 0 || splitted.size() != 4) 
							return false;
						std::cout << splitted[0] << " " << splitted[1] << " " << splitted[2] << " " << splitted[3] << std::endl;
						for (int i = 1; i <= 3; i++) {
							offest.push_back(stoi(splitted[i]));
						}
					} else if (splitted[0] == "CHANNELS") {
						if (channels.size() != 0 || splitted.size() != 8) 
							return false;
						if (splitted[1] != "6" || splitted[2] != "Xposition" || splitted[3] != "Yposition" || splitted[4] != "Zposition" || splitted[5] != "Zrotation" || splitted[6] != "Yrotation" || splitted[7] != "Xrotation")
							return false;
						for (int i = 2; i <= 7; i++) {
							channels.push_back(splitted[i]);
						}
						std::cout << splitted[2] << " " << splitted[3] << " " << splitted[4] << " " << splitted[5] << " " << splitted[6] << " " << splitted[7] << std::endl;
					} else if (splitted[0] == "JOINT") {
						if (next.length() != 0 || splitted.size() != 2) 
							return false;
						next = splitted[1];
						std::cout << splitted[1] << std::endl;
					} else if ((splitted[0] == "}" || splitted[0] == "{")) {
						return false;
					}
				}
			}
			splitted = split(line);
			if (splitted.size() == 0) {
				continue;
			}
			if (splitted[0] == "{") {
				std::cout << splitted[0] << std::endl;
				this->_root = new Root(channels);
				if (parseMember(next) == false)
					return false;
			} else if (splitted[0] == "}" && this->_root == nullptr) {
				this->_root = new Root(channels);
				std::cout << "root created" << std::endl;
				return true;
			} else if (splitted[0] == "}") {
				return true;
			}
			else {
				return false;
			}
		} else {
			return false;
		}
	}

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
			if (offest.size() != 0 || splitted.size() != 4) 
				return false;
			std::cout << splitted[0] << " " << splitted[1] << " " << splitted[2] << " " << splitted[3] << std::endl;
			for (int i = 1; i <= 3; i++) {
				offest.push_back(stoi(splitted[i]));
			}
		} else if (splitted[0] == "CHANNELS") {
			if (channels.size() != 0 || splitted.size() != 5) 
				return false;
			if (splitted[1] != "3" || splitted[2] != "Zrotation" || splitted[3] != "Yrotation" || splitted[4] != "Xrotation")
				return false;
			for (int i = 2; i <= 4; i++) {
				channels.push_back(splitted[i]);
			}
			std::cout << splitted[2] << " " << splitted[3] << " " << splitted[4] << std::endl;
		} else if (splitted[0] == "JOINT") {
			if (next.length() != 0 || splitted.size() != 2) 
				return false;
			next = splitted[1];
			std::cout << splitted[1] << std::endl;
		} else if (splitted[0] == "End" && splitted[1] == "Site" && next.length() == 0) {
			endSite = true;
			std::cout << splitted[0] << std::endl;
			if (parseEndSite() == false) {
				return false;
			}
		} else if ((splitted[0] == "}" || splitted[0] == "{")) {
			return false;
		}
	}
	splitted = split(line);
	
	if (splitted.size() != 0) {
		if (splitted[0] == "{") {
			// cree le membre
			std::cout << splitted[0] << std::endl;
			if (parseMember(next) == false)
			return false;
		} else if (splitted[0] == "}" && offest.size() != 0 && channels.size() != 0 && next.length() == 0 && endSite == true) {
			std::cout << splitted[0] << std::endl;
			return true;
		} else if (splitted[0] == "}" && (offest.size() == 0 || channels.size() == 0 || next.length() == 0)) {
			return false;
		} else if (splitted[0] == "}") {
			std::cout << splitted[0] << std::endl;
			return true;
		} else {
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
			if (parseMember(next) == false)
				return false;
		} else if (splitted[0] == "}" && offest.size() != 0 && channels.size() != 0 && next.length() == 0 && endSite == true) {
			std::cout << splitted[0] << std::endl;
			return true;
		} else if (splitted[0] == "}" && (offest.size() == 0 || channels.size() == 0 || next.length() == 0)) {
			return false;
		} else if (splitted[0] == "}") {
			std::cout << splitted[0] << std::endl;
			return true;
		} else {
			return false;
		}
	}

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
	return false;
}
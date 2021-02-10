#ifndef _RULES_H
# define _RULES_H

struct Rule {
	std::string name;
	std::vector<std::string> dependencies;
	std::vector<std::string> commands;
};

# endif
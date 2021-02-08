#ifndef _PARSER_H
# define _PARSER_H

#include "options.h"
#include <vector>
#include <string>
#include <set>
#include <algorithm>
#include <filesystem>

class Parser {
public:
	Parser(int ac, char **av);
	~Parser();

	Options parse_args();

private:

	void c_flag_(Options& opt, size_t& i);
	void f_flag_(Options& opt, size_t& i);
	void s_flag_(Options& opt, size_t& i);
	void set_flags_(Options& opt);
	void separate_tokens_(Options& opt);


	std::vector<std::string> av_;
	std::set<std::string> opt_;
};

#endif
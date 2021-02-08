#ifndef _INITMK_H
# define _INITMK_H

#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <sys/stat.h>

#include "rules.h"
#include "variable.h"
#include "options.h"

#define MAKEFILE "MKFL"

#define OPEN_ERROR "An error occured while opening the file."

class Initmk {
public:
	Initmk();
	~Initmk();
	
	void initmk(Options& opt);


private:

	inline static const std::string SRC_DIRECTORY[5] = {"src", "srcs", "sources", "source"};

	inline static Options::C_CXX type_file(const std::string& file_name) {
		if (file_name.substr(file_name.size() - 3) == ".c") {
			return Options::C;
		} else if (file_name.substr(file_name.size() - 4) == ".cpp") {
			return Options::CXX;
		}
		return Options::Unknown;
	}
	static bool is_sources_directory(std::string s) {for (auto str: SRC_DIRECTORY) if (str == s) { return true; } return false; }

	void set_variables_();
	void set_compiler_variables_();

	void write_makefile_() const;

	void verify_sources_() const;
	void find_sources_(const std::string& path);

	std::vector<rules> rules_;
	std::vector<variable> variables_;

	Options* opt_;
};

#endif
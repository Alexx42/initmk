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

	void set_variables_();
	void set_compiler_variables_();

	void write_makefile_() const;

	void verify_sources_() const;

	std::vector<rules> rules_;
	std::vector<variable> variables_;

	Options* opt_;
};

#endif
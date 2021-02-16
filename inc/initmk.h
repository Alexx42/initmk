#ifndef _INITMK_H
# define _INITMK_H

#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <sys/stat.h>
#include <numeric>
#include <map>
#include <utility>

#include "rules.h"
#include "variable.h"
#include "options.h"
#include "files.h"

#define MAKEFILE "Mkfl"
#define SRC_VAR "SRC"
#define OBJ_VAR "OBJ"
#define OBJ_DIR "obj"
#define OBJECT_OPTION "-c"
#define RENAME_OPTION "-o"
#define TARGET "$@"
#define FIRST_PREREQUISITE "$<"

#define OPEN_ERROR "An error occured while opening the file."

typedef std::vector<std::string> StringList;


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

	void create_rules_();

	void make_clean_rule();

	std::vector<Rule> rules_;
	std::vector<Variable> variables_;

	std::string compiler_variable_;
	std::string flags_variable_;

	std::string path_;

	Files files_;

	Options* opt_;
};

#endif
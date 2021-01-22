#ifndef _INITMK_H
# define _INITMK_H

#include <string>
#include <vector>

#include "rules.h"
#include "variable.h"
#include "options.h"

class Initmk {
public:
	Initmk();
	~Initmk();
	
	void initmk(Options& opt);
private:

	void set_variables_();
	void set_compiler_variables_();

	std::vector<rules> rules_;
	std::vector<variable> variables_;

	Options* opt_;
};

#endif
#include "../inc/initmk.h"

Initmk::Initmk()  {
	;
} 

Initmk::~Initmk() {
	;
}

void Initmk::set_compiler_variables_() {
	if (opt_->is_c_set()) {
		std::string name;
		if (opt_->compiler == "clang" || opt_->compiler == "gcc") {
			name = "CC";
		} else {
			name = "CXX";
		}
		variables_.push_back({name, opt_->compiler});
	}
}

void Initmk::set_variables_() {
	variables_.push_back({"NAME", opt_->name});
	set_compiler_variables_();
}

void Initmk::initmk(Options& opt) {
	opt_ = &opt;

	set_variables_();
	for (auto x: variables_) {
		std::cout << x.name << " = " << x.value << std::endl;
	}
}
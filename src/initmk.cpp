#include "../inc/initmk.h"

Initmk::Initmk()  {
	;
} 

Initmk::~Initmk() {
	;
}

void Initmk::set_compiler_variables_() {
	std::string flags;

	if (opt_->is_c_set()) {
		std::string name;
		if (opt_->compiler == "clang" || opt_->compiler == "gcc") {
			name = "CC";
		} else {
			name = "CXX";
		}
		variables_.push_back({name, opt_->compiler});
	}
	for (auto& f: opt_->c_flags) {
		flags += f + ' ';
	}
	if (opt_->compiler == "clang" || opt_->compiler == "gcc") {
		variables_.push_back({"CFLAGS", flags});
	} else {
		variables_.push_back({"CXXFLAGS", flags});
	}
}

void Initmk::set_variables_() {
	variables_.push_back({"NAME", opt_->name});
	set_compiler_variables_();
}

void Initmk::write_makefile_() const {
	std::ofstream file;

	file.open(MAKEFILE, std::ios::trunc);

	if (file.fail()) {
		throw std::runtime_error(OPEN_ERROR);
	}
	for (auto& var: variables_) {
		file << var.name << "\t\t = \t\t" << var.value << "\n";
	}
	file.close();
}

void Initmk::verify_sources_() const {
	std::string current_path;
	if (!(opt_->is_s_set())) {
		return ;
	}
	current_path = std::filesystem::current_path().string();
	struct stat buffer{};

	for (auto& file : opt_->sources) {
		if (stat((current_path.append('/' + file)).c_str(), &buffer) != 0) {
			throw std::runtime_error("Could not find the following file: " + file);
		}
	}
}

void Initmk::initmk(Options& opt) {
	opt_ = &opt;
	try {
		verify_sources_();
		set_variables_();
		write_makefile_();
	} catch (std::runtime_error& e) {
		throw e;
	}
}
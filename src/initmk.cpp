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
		opt_->lg = Options::C;
	} else {
		variables_.push_back({"CXXFLAGS", flags});
		opt_->lg = Options::CXX;
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

	current_path = std::filesystem::current_path().string();
	struct stat buffer{};

	for (auto& file : opt_->sources) {
		if (stat((current_path + '/' + file).c_str(), &buffer) != 0) { // NOLINT(performance-inefficient-string-concatenation)
			throw std::runtime_error("Could not find the following file: " + file);
		}
	}
	if (opt_->lg == Options::C) {
		for (auto& file : opt_->sources) {
			if (file.substr(file.size() - 3) != ".c") {
				throw std::runtime_error("Impossible to compile the following file: " + file + " under " + opt_->compiler);
			}
		}
	}
}

void Initmk::find_sources_(const std::string& path) {
	std::string p;
	for (auto& entry : std::filesystem::directory_iterator(path)) {
		if (entry.is_directory() && is_sources_directory(entry.path().string().substr(path.size() + 1))) {
			for (auto& files : std::filesystem::directory_iterator(entry.path().string())) {
				if (!files.is_regular_file()) {
					continue ;
				}
				p = files.path().string();
				if (type_file(p) == Options::C) {
					if (opt_->lg == Options::Unknown) { opt_->lg = Options::C; }
					opt_->sources.push_back(p);
				} else if (type_file(p) == Options::CXX) {
					if (opt_->lg == Options::Unknown) { opt_->lg = Options::CXX; }
					opt_->sources.push_back(p);
				}
			}
			find_sources_(entry.path().string());
		}
	}
}

void Initmk::initmk(Options& opt) {
	opt_ = &opt;
	set_variables_();
	try {
		if (opt.is_s_set()) {
			verify_sources_();
		} else {
			find_sources_(std::filesystem::current_path().string());
			for (auto& src: opt_->sources) {
				std::cout << src << std::endl;
			}
		}
		write_makefile_();
	} catch (std::runtime_error& e) {
		throw e;
	}
}
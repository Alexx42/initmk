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
	if (!(opt_->is_f_set())) { return; }

	if (opt_->compiler == "clang" || opt_->compiler == "gcc") {
		variables_.push_back({"CFLAGS", flags});
		opt_->lg = Options::C;
	} else {
		variables_.push_back({"CXXFLAGS", flags});
		opt_->lg = Options::CXX;
	}
}

template<class InputIt, class T>
constexpr // since C++20
T p_accumulate(InputIt first, InputIt last, T init)
{
	for (; first != last; ++first) {
		init = std::move(init) + *first + ' '; // std::move since C++20
	}
	return init;
}

void Initmk::set_variables_() {
	variables_.push_back({"NAME", opt_->name});

	Variable sources;

	sources.name = SRC_VAR;
	sources.value = p_accumulate(opt_->sources.begin(), opt_->sources.end(), std::string{});
	variables_.push_back(sources);

	Variable object;

	object.name = OBJ_VAR;
	if (opt_->lg == Options::C) {
		object.value = std::string("$(") + SRC_VAR + ":.c=.o)";
	} else {
		object.value = std::string("$(") + SRC_VAR + ":.cpp=.o)";
	}
	variables_.push_back(object);

	Variable object_dir;

	object_dir.name = "OBJ_DIR";
	object_dir.value = OBJ_DIR;
	variables_.push_back(object_dir);

	set_compiler_variables_();
}

void Initmk::write_makefile_() const {
	std::ofstream file;

	file.open(MAKEFILE, std::ios::trunc);

	if (file.fail()) {
		throw std::runtime_error(OPEN_ERROR);
	}
	for (auto& var: variables_) {
		file << var.name << " = " << var.value << "\n";
	}
	file << "\n\n";
	for (auto& rule: rules_) {
		file << rule.name << " : ";
		for (const auto& dependency : rule.dependencies) {
			file << dependency << " ";
		}
		file << "\n";
		for (const auto& command : rule.commands) {
			file << "\t" << command << "\n";
		}
	}
	file.close();
}

void Initmk::verify_sources_() const {
	std::string current_path;
	bool cpp = false, c = false;
	current_path = std::filesystem::current_path().string();
	struct stat buffer{};

	for (auto& file : opt_->sources) {
		std::string extension = file.substr(file.find('.') + 1);
		if (extension == "cpp") {
			cpp = true;
		} else if (extension == "c") {
			c = true;
		} else {
			throw std::runtime_error("Invalid type of file: " + file);
		}
		if (stat((current_path + '/' + file).c_str(), &buffer) != 0) { // NOLINT(performance-inefficient-string-concatenation)
			throw std::runtime_error("Could not find the following file: " + file);
		}
	}
	if (opt_->lg == Options::C && cpp) {
		throw std::runtime_error("Impossible to compile a C++ file under a C compiler.");
	} else if (!cpp && !c) {
		throw std::runtime_error("Invalid number of files.");
	} else if (opt_->lg == Options::Unknown) {
		if (c && cpp) {
			opt_->lg = Options::CXX;
		} else if (c) {
			opt_->lg = Options::C;
		} else {
			opt_->lg = Options::CXX;
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

void Initmk::make_clean_rule() {

}

void Initmk::create_rules_() {
	Rule recompile;

	recompile.name = "$(OBJ_DIR)/%.o";
	recompile.dependencies.emplace_back("%.c");
	recompile.commands.emplace_back(compiler_variable + " " + flags_variable + " " +
	OBJECT_OPTION + " " + FIRST_PREREQUISITE + " " + RENAME_OPTION + " " + TARGET);

	rules_.push_back(recompile);
}

void Initmk::initmk(Options& opt) {
	opt_ = &opt;
	set_variables_();
	try {
		if (opt.is_s_set()) {
			verify_sources_();
		} else {
			find_sources_(std::filesystem::current_path().string());
		}
		if (opt_->lg == Options::C) {
			compiler_variable = "$(CC)";
			flags_variable = "$(CFLAGS)";
		} else {
			compiler_variable = "$(CXX)";
			flags_variable = "$(CXXFLAGS)";
		}
		create_rules_();
		write_makefile_();
	} catch (std::runtime_error& e) {
		throw e;
	}
}
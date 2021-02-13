#include "../inc/initmk.h"

Initmk::Initmk() : opt_(nullptr) {
	path_ = std::filesystem::current_path().string();
}

Initmk::~Initmk() {
	;
}

void Initmk::set_compiler_variables_() {
	std::string flags;

	if (opt_->lg == Options::C) {
		if (opt_->compiler.empty()) {
			opt_->compiler = "gcc";
		}
		variables_.push_back({"CC", opt_->compiler});
	} else {
		if (opt_->compiler.empty()) {
			opt_->compiler = "g++";
		}
		variables_.push_back({"CXX", opt_->compiler});
	}
	if (!(opt_->is_f_set())) { return; }

	for (auto& f: opt_->c_flags) {
		flags += f + ' ';
	}

	if (opt_->lg == Options::C) {
		variables_.push_back({"CFLAGS", flags});
		opt_->lg = Options::C;
	} else {
		variables_.push_back({"CXXFLAGS", flags});
		opt_->lg = Options::CXX;
	}
}

template<class InputIt, class T>
constexpr
T p_accumulate(InputIt first, InputIt last, T init)
{
	for (; first != last; ++first) {
		init = std::move(init) + *first + ' ';
	}
	return init;
}

void Initmk::set_variables_() {
	variables_.push_back({"NAME", opt_->name});

	set_compiler_variables_();

	Variable source;

	const std::vector<std::string>& sources = files_.get_sources();

	source.name = SRC_VAR;
	source.value = p_accumulate(sources.begin(), sources.end(), std::string{});
	variables_.push_back(source);

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

	Variable include;

	const std::vector<std::string>& includes = files_.get_includes();

	include.name = "INCLUDES";
	include.value = p_accumulate(includes.begin(), includes.end(), std::string{});
	variables_.push_back(include);


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
		file << "\n";
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


void Initmk::make_clean_rule() {

}

void Initmk::create_rules_() {
	Rule all;

	all.name = "all";
	all.dependencies.emplace_back("$(NAME)");

	rules_.push_back(all);

	Rule recompile;

	recompile.name = "$(OBJ)";
	if (opt_->lg == Options::C) {
		recompile.dependencies.emplace_back("$(OBJ_DIR)/%.o : %.c");
	} else {
		recompile.dependencies.emplace_back("$(OBJ_DIR)/%.o : %.cpp");
	}
	recompile.commands.emplace_back("@mkdir -p $(OBJ_DIR)");
	recompile.commands.emplace_back(compiler_variable_ + " -I $(INCLUDES) " + flags_variable_ + " " +
	OBJECT_OPTION + " " + FIRST_PREREQUISITE + " " + RENAME_OPTION + " " + TARGET);

	rules_.push_back(recompile);

	Rule name;

	name.name = "$(NAME)";
	name.dependencies.emplace_back(std::string("$(") + OBJ_VAR + ")");
	name.commands.emplace_back(compiler_variable_ + " -I $(INCLUDES) " + flags_variable_ + " " + RENAME_OPTION + " $@ " + TARGET);

	rules_.push_back(name);

	Rule clean;

	clean.name = "clean";
	clean.commands.emplace_back(std::string("@rm -f $(") + OBJ_DIR + ")/*.o");
	clean.commands.emplace_back(std::string("@rm -f $(NAME)"));

	rules_.push_back(clean);


}

void Initmk::initmk(Options& opt) {
	opt_ = &opt;
	try {
		if (opt.is_s_set()) {
			verify_sources_();
			files_.set_sources(opt_->sources);
		} else {
			files_.get_all_sources_files(std::filesystem::current_path().string());
		}
		files_.get_all_include_directories(path_);
		set_variables_();
		if (opt_->lg == Options::C) {
			compiler_variable_ = "$(CC)";
			flags_variable_ = "$(CFLAGS)";
		} else {
			compiler_variable_ = "$(CXX)";
			flags_variable_ = "$(CXXFLAGS)";
		}
		create_rules_();
		write_makefile_();
	} catch (std::runtime_error& e) {
		throw e;
	}
}
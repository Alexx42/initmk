#include "../inc/parser.h"

Parser::Parser(int ac, char **av) {
	std::string s;
    for (int i = 1; i < ac; ++i) {
		s = av[i];
		std::transform(s.begin(), s.end(), s.begin(), ::tolower);
        av_.push_back(s);
    }
}

Parser::~Parser() {
    ;
}

void Parser::separate_tokens(Options& opt) {
	size_t i;

	if (av_.size() == 0 || av_[0][0] == '-') {
		throw std::runtime_error("usage: ./initmk <projectname> -c <COMPILER NAME> -s <SOURCES FILES>.");
	}
	opt.name = av_[0];
	for (i = 0; i < av_.size(); ++i) {
		if (av_[i].size() == 2 && av_[i][0] == '-' && av_[i][1] != '-') {
			if (!Options::is_opt(av_[i][1])) {
				throw std::runtime_error("invalid option name.");
			}
			opt_.insert(av_[i]);
			if (av_[i] == "-c") {
				if (i + 1 >= av_.size()) {
					throw std::runtime_error("usage: ./initmk <projectname> -c <COMPILER NAME> -s <SOURCES FILES>.");
				} else if (!Options::is_compiler(av_[i + 1])) {
					throw std::runtime_error("invalid compiler name.");
				}
				opt.compiler = av_[i + 1];
			}
		} else if (av_[i][0] == '-') {
			throw std::runtime_error("invalid option name.");
		}
    }
}

void Parser::set_flags_(Options& opt) {
    for (auto& elem: opt_) {
        if (elem == "-s") {
            opt.set_s_flag();
        } else if (elem == "-c") {
			opt.set_c_flag();
		}
    }
}

Options Parser::parse_args() {
    Options opt;

	try {
		separate_tokens(opt);
		opt.set_nb_flags(opt_.size());
		set_flags_(opt);
	} catch (std::runtime_error& e) {
		throw e;
	}
    return opt;
}
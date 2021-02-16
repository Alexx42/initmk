#include "../inc/parser.h"

Parser::Parser(int ac, char **av) {
	std::string s;
    for (int i = 1; i < ac; ++i) {
		s = av[i];
		av_.push_back(s);
	}
}

Parser::~Parser() {
    ;
}


void Parser::c_flag_(Options& opt, size_t& i) {
	if (i >= av_.size() - 1) {
		throw std::runtime_error("usage: ./initmk <NAME> -c <COMPILER> -s <SOURCES FILES>.");
	} else if (!Options::is_compiler(av_[i + 1])) {
		throw std::runtime_error("invalid compiler name.");
	}
	++i;
	opt.compiler = av_[i];
}

void Parser::f_flag_(Options& opt, size_t& i) {
	if (i >= av_.size() - 1) {
		throw std::runtime_error(USAGE);
	}
	++i;
	uint8_t j = 0, n_letter;
	while (j < av_[i].size()) {
		n_letter = 0;
		while (j < av_[i].size() && isspace(av_[i][j])) {
			++j;
		}
		while (j < av_[i].size() && !isspace(av_[i][j])) {
			++j;
			++n_letter;
		}
		opt.c_flags.push_back(av_[i].substr(j - n_letter, n_letter));
	}
}

void Parser::s_flag_(Options& opt, size_t& i) {
	if (i >= av_.size() - 1) {
		throw std::runtime_error(USAGE);
	}
	while (++i < av_.size() && av_[i][0] != '-') {
		opt.sources[""].push_back(av_[i]);
	}
}

void Parser::separate_tokens_(Options& opt) {
	size_t i;

	if (av_.empty() || av_[0][0] == '-') {
		throw std::runtime_error(USAGE);
	}
	opt.name = av_[0];
	for (i = 0; i < av_.size(); ++i) {
		if (av_[i].size() == 2 && av_[i][0] == '-' && av_[i][1] != '-') {
			if (!Options::is_opt(av_[i][1])) {
				throw std::runtime_error("invalid option name.");
			}
			opt_.insert(av_[i]);
			try {
				if (av_[i] == "-c") {
					c_flag_(opt, i);
				} else if (av_[i] == "-f") {
					f_flag_(opt, i);
				} else if (av_[i] == "-s") {
					s_flag_(opt, i);
				}
			} catch (std::runtime_error& e) {
				throw e;
			}
		} else if (av_[i][0] == '-') {
			if (!(i > 0 && av_[i - 1] == "-f")) {
				throw std::runtime_error("invalid option name.");
			}
		}
    }
}

void Parser::set_flags_(Options& opt) {
    for (auto& elem: opt_) {
        if (elem == "-s") {
            opt.set_s_flag();
        } else if (elem == "-c") {
			opt.set_c_flag();
		} else if (elem == "-f") {
			opt.set_f_flag();
		}
    }
}

Options Parser::parse_args() {
    Options opt;

	try {
		separate_tokens_(opt);
		opt.set_nb_flags(opt_.size());
		set_flags_(opt);
	} catch (std::runtime_error& e) {
		throw e;
	}
    return opt;
}
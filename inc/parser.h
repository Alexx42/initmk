#ifndef _PARSER_H
# define _PARSER_H

#include "options.h"
#include <vector>
#include <string>
#include <set>
#include <algorithm>

class Parser {
public:
    Parser(int ac, char **av);
    ~Parser();

    Options parse_args();

private:

    void    set_flags_(Options& opt);
    void separate_tokens(Options& opt);

    std::vector<std::string> av_;
    std::set<std::string> opt_;
};

#endif
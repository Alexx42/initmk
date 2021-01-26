#include <iostream>
#include "../inc/initmk.h" 
#include "../inc/options.h"
#include "../inc/parser.h"

int main(int ac, char **av) {
	Initmk mk;
	Parser parser(ac, av);

	try {
		Options opt = parser.parse_args();
		for (size_t i = 0; i < opt.c_flags.size(); ++i) {
			std::cout << opt.c_flags[i] << std::endl;
		}
		mk.initmk(opt);
	} catch (std::runtime_error &e) {
		std::cout << e.what() << std::endl;
		return 1;
	}
	return 0;
}
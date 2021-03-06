#include <iostream>

#include "../inc/initmk.h"
#include "../inc/options.h"
#include "../inc/parser.h"


int main(int ac, char **av) {
	Initmk mk;
	Parser parser(ac, av);

	try {
		Options opt = parser.parse_args();
		mk.initmk(opt);
	} catch (std::runtime_error &e) {
		std::cout << e.what() << std::endl;
		return 1;
	}
	return 0;
}
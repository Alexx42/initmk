#ifndef _OPTIONS_H
# define _OPTIONS_H

#include <cstdint>
#include <set>
#include <iostream>

# define S_FLAG 0x02
# define C_FLAG 0x03

struct Options {


	Options();
	~Options();

	void set_nb_flags(uint8_t n);

	inline void set_s_flag() { flags |= (1 << S_FLAG); }
	inline void set_c_flag() { flags |= (1 << C_FLAG); }

	constexpr bool is_s_set() { return flags & (1 << S_FLAG); }
	constexpr bool is_c_set() { return flags & (1 << C_FLAG); }

	/*
	** 8 bits representing flags, respectively:
	** two first bits represents the number of flags
	** third bit corresponds to the s flag
	** fourth bit corresponds to the c flag
	*/
	uint8_t flags;

	std::string name;
	std::string compiler;

	static const uint8_t MAX_FLAGS = 2;
	static constexpr char OPT[2] {'s', 'c'};
	inline static const std::string COMPILER[5] {"gcc", "g++", "clang", "clang++"};

	static bool is_opt(char c) { for (auto x: OPT) if (x == c) { return true; } return false; }
	static bool is_compiler(std::string s) {for (auto str: COMPILER) if (str == s) { return true; } return false; }
};

#endif
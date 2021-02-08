#include "../inc/options.h"


Options::Options() : flags(0x00), lg(Unknown) { ; }

Options::~Options() { ; }

void Options::set_nb_flags(uint8_t n) {
    if (n > Options::MAX_FLAGS) {
        throw std::runtime_error("Invalid number of flags");
    }
    flags |= n;
}



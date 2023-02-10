//
// Created by richard on 09/02/23.
//

/*
 * BMain.cpp Created by Richard Buckley (C) 09/02/23
 */

/**
 * @file BMain.cpp
 * @author Richard Buckley <richard.buckley@ieee.org>
 * @version 1.0
 * @date 09/02/23
 */

#include "BMain.h"

int main(const int argc, char const * const * const argv) {
    std::vector<std::string_view> args(argv, std::next(argv, static_cast<std::ptrdiff_t>(argc)));
    return better_main::start(args);
}

namespace better_main {
} // better_main
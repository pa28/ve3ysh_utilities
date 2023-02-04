//
// Created by richard on 03/02/23.
//

/*
 * ostring.h Created by Richard Buckley (C) 03/02/23
 */

/**
 * @file StringComposite.h
 * @author Richard Buckley <richard.buckley@ieee.org>
 * @version 1.0
 * @date 03/02/23
 * @brief 
 * @details
 */

#ifndef YSH_STRCOMPOSITE_H
#define YSH_STRCOMPOSITE_H

#include <string>
#include <sstream>

namespace ysh {

    /**
    * @brief Composite a pack of arguments that are streamable to a string.
    * @tparam Arg First argument type
    * @tparam Args Rest of the argument types
    * @param arg First argument
    * @param args Rest of the arguments
    * @return The composited string
    */
    template<typename Arg, typename... Args>
    std::string StringComposite(Arg &&arg, Args &&... args) {
        std::stringstream out;
        out << std::forward<Arg>(arg);
        ((out << std::forward<Args>(args)), ...);
        return out.str();
    }

} // ysh

#endif //YSH_STRCOMPOSITE_H

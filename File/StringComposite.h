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
     * @struct StringAppend
     * @brief Derived from std::string adding the capability of appending simple deduced types.
     * @details This is used with StringComposite to build up a string from a variadic argument pack.
     * Any argument that satisfies std::string::append(arg), std::string::append(1,char) or
     * std::to_string(arg) can be concatenated together to form a std::string.
     */
    struct StringAppend : public std::string {
        template<class Arg>
        void appendArg(Arg arg) { append(arg); }

        [[maybe_unused]] void appendArg(char& arg) { append(1,arg); }

        template<class Arg>
        requires std::is_floating_point_v<Arg> || std::is_integral_v<Arg>
        void appendArg(Arg arg) { append(std::to_string(arg)); }
    };

    /**
     * @brief Composite a pack of arguments that are streamable to a string.
     * @details Uses StringAppend to build up the final string. This may be lighter weight than
     * StringStreamComposite() but has less utility.
     * @tparam Arg First argument type
     * @tparam Args Rest of the argument types
     * @param arg First argument
     * @param args Rest of the arguments
     * @return The composited string
     */
    template<typename Arg, typename... Args>
    [[maybe_unused]] std::string StringComposite(Arg &&arg, Args &&... args) {
        StringAppend out{};
        out.appendArg(std::forward<Arg>(arg));
        (out.appendArg(std::forward<Args>(args)), ...);
        return out;
    }

    /**
     * @brief Composite a pack of arguments that are streamable to a string.
     * @details Uses std::stringstream to build up the final string.
     * @tparam Arg First argument type
     * @tparam Args Rest of the argument types
     * @param arg First argument
     * @param args Rest of the arguments
     * @return The composited string
     */
    template<typename Arg, typename... Args>
    [[maybe_unused]] std::string StringStreamComposite(Arg &&arg, Args &&... args) {
        std::stringstream out{};
        out << std::forward<Arg>(arg);
        ((out << std::forward<Args>(args)), ...);
        return out.str();
    }

} // ysh

#endif //YSH_STRCOMPOSITE_H

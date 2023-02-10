/*
 * BMain.h Created by Richard Buckley (C) 09/02/23
 */

/**
 * @file BMain.h
 * @author Richard Buckley <richard.buckley@ieee.org>
 * @version 1.0
 * @date 09/02/23
 * @brief 
 * @details
 */

#ifndef VE3YSH_UTIL_BMAIN_H
#define VE3YSH_UTIL_BMAIN_H

#include <span>
#include <string_view>
#include <vector>
#include <string>
#include <filesystem>
#include <iostream>
#include <exception>
#include <ranges>
#include <StringComposite.h>
#include <algorithm>

namespace better_main {

    /**
     * @brief Forward declaration of the starting function.
     * @details A replacement main() function compiles the program arguments into a span of string_view. This
     * version of main does not catch any exceptions. Any that are not caught by the time they get to start()
     * will cause the program to abort.
     * @return
     */
    [[nodiscard]] int start(std::span<const std::string_view>) noexcept;

    /**
     * @enum ArgType
     * @brief The type of the associated value of the option.
     * @details The type is not enforced until the program requests the translated value from the support library.
     */
    enum class ArgType {
        NoValue,        ///< The option takes no value argument, it is either seen or not.
        Help,           ///< The help option takes an optional argument which is an option for which help is sought.
        String,         ///< The option takes a string argument.
        Integer,        ///< The option takes an integer argument.
        Float,          ///< The option takes a floating point argument.
        Path,           ///< The option takes a filesystem path argument.
    };

    /**
     * @struct BMainArg
     * @brief The structure that holds values associated with each option.
     * @details The user program will normally create a static constexpr array of these structures that are
     * passed to the support library for option processing.
     * @tparam Enum A user supplied enumeration that identifies options.
     */
    template<class Enum>
    struct BMainArg {
        Enum argIdx{};                  ///< Identifier of the option
        ArgType argType{};              ///< The type of argument the option takes
        char shortArg{};                ///< The short (single character) option
        std::string_view longArg{};     ///< The long option
        std::string_view shortHelp{};   ///< Short (less than one line) help string.
        std::string_view longHelp{};    ///< A longer help string.
    };

    template<class Range, class Enum>
    requires std::is_enum_v<Enum>
    auto findOption(const Range args, Enum arg) {
        return std::ranges::find_if(args, [&arg](auto opt) {
            return opt.argIdx == arg;
        });
    }

    template<class Range>
    auto findOption(const Range args, char arg) {
        return std::ranges::find_if(args, [&arg](auto opt) {
            return opt.shortArg == arg;
        });
    }

    template<class Range, class String>
    requires std::is_same_v<String,std::string> || std::is_same_v<String,std::string_view>
    auto findOption(const Range args, const String& arg) {
        return std::ranges::find_if(args, [&arg](auto opt) { return opt.longArg == arg; });
    }

    /**
     * @struct BMainArgValue
     * @brief The structure that holds the option observations from the program invocation.
     * @details If the option takes an argument the string is copied into value.
     * @tparam Enum A user supplied enumeration that identifies options.
     */
    template<class Enum>
    struct BMainArgValue {
        Enum argIdx{};              ///< Identifier of the option
        std::string value{};        ///< The argument of the option
    };

    /**
     * @struct Invocation
     * @brief A type to hold the option observations gathered from the program invocation.
     * @details Invocation is derived from std::vector of BMainArgValue<Enum> to store the list of option observations.
     * It also holds the program executable path and a list of free arguments which are not tied to an option.
     * Observations and free arguments are saved in the order they are encountered.
     * @tparam Enum A user supplied enumeration that identifies options.
     */
    template<class Enum>
    struct Invocation : public std::vector<BMainArgValue<Enum>> {
        std::filesystem::path programPath{};    ///< Executable path.
        std::vector<std::string> freeArgs{};    ///< Free and positional arguments.
    };

    template<class Range, class Enum>
    requires std::ranges::range<Range>
    std::optional<BMainArgValue<Enum>> findArgument(const Range args, Enum arg) {
        if (auto r = std::ranges::find_if(args, [&arg](auto opt) { return opt.argIdx == arg; }); r != args.end())
            return *r;
        return std::nullopt;
    }

    /**
     * @brief Exception thrown on an unrecoverable command line option parsing error.
     */
    class ArgParseError : public std::runtime_error {
    public:
        explicit ArgParseError(const std::string& whatArg) : std::runtime_error(whatArg) {}
    };

    /**
     * @brief Parse command line arguments into an Invocation structure.
     * @tparam Enum A user supplied enumeration that identifies options.
     * @tparam Size The size of the option array, deduced.
     * @param args The arguments provided to start().
     * @param argSpec The option array.
     * @return An Invocation<Enum> structure with invocation observations.
     * @throws ArgParseError.
     */
    template<class Enum,std::size_t Size>
    Invocation<Enum> parseArgs(std::span<const std::string_view>& args, const std::array<BMainArg<Enum>,Size>& argSpec){
        using ArgListIterator = std::array<BMainArg<Enum>,Size>::const_iterator;
        bool doubleDash{false};

        Invocation<Enum> invocation{};
        invocation.programPath = args.front();
        auto argList = args.subspan<1>();
        for (size_t idx = 0; idx < argList.size(); ++idx) {
            if (doubleDash) {
                invocation.freeArgs.emplace_back(std::string(argList[idx]));
            } else {
                ArgListIterator valuedOption = argSpec.end();
                auto argString = argList[idx];
                if (!argString.empty() && argString[0] == '-') {
                    if (argString.size() > 1 && argString[1] == '-') {
                        if (argString.size() == 2) {
                            doubleDash = true;
                        } else {
                            ArgListIterator argItem = findOption(argSpec, argString.substr(2));
                            if (argItem != argSpec.end()) {
                                if (argItem->argType != ArgType::NoValue)
                                    valuedOption = argItem;
                                else
                                    invocation.emplace_back(argItem->argIdx);
                            } else {
                                throw ArgParseError("Command line option not found.");
                            }
                        }
                    } else {
                        bool valueUsed{false};
                        for (auto argChar: argString.substr(1)) {
                            ArgListIterator argItem = findOption(argSpec, argChar);
                            if (argItem != argSpec.end()) {
                                if (argItem->argType != ArgType::NoValue) {
                                    if (!valueUsed) {
                                        valuedOption = argItem;
                                        valueUsed = true;
                                    } else {
                                        throw ArgParseError(ysh::StringComposite("In option '", argString,
                                                                                 "' more than one option takes an argument."));
                                    }
                                } else {
                                    invocation.emplace_back(argItem->argIdx);
                                }
                            } else {
                                throw ArgParseError(
                                        ysh::StringComposite("Command line option '", argChar, "' not found."));
                            }
                        }
                    }

                    if (valuedOption != argSpec.end()) {
                        if (argList.size() > idx + 1) {
                            ++idx;
                            invocation.emplace_back(valuedOption->argIdx, std::string(argList[idx]));
                        } else
                            throw ArgParseError(ysh::StringComposite("Command line option '", valuedOption->longArg,
                                                                     "' takes a value but none is provided."));
                    }
                } else {
                    invocation.freeArgs.emplace_back(std::string(argList[idx]));
                }
            }
        }
        return invocation;
    }

    /**
     * @brief Count the number of occurrences of an option in the invocation.
     * @tparam Enum A user supplied enumeration that identifies options.
     * @param arg The option to count.
     * @param invocation The option observations from argument parsing.
     * @return The count.
     */
    template<class Enum>
    [[maybe_unused]] auto occurrenceCount(Enum arg, const Invocation<Enum>& invocation) {
        return std::ranges::count_if(invocation, [&arg](auto opt) { return opt.argIdx == arg; } );
    }

} // better_main

int main(int argc, char const * const * argv);

#endif //VE3YSH_UTIL_BMAIN_H

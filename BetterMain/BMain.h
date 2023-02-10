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
#include <StringComposite.h>

namespace better_main {

    [[nodiscard]] int start(std::span<const std::string_view>) noexcept;

    enum class ArgType {
        NoValue, Help, String, Integer, Float, Path,
    };

    /**
     * @struct BMainArgs
     */
    template<class Enum>
    struct BMainArg {
        Enum argIdx{};
        ArgType takesValue{};
        char shortArg{};
        std::string_view longArg{};
        std::string_view shortHelp{};
        std::string_view longHelp{};
    };

    template<class Enum>
    struct BMainArgValue {
        Enum argIdx{};
        std::string value{};
    };

    template<class Enum>
    struct Invocation : public std::vector<BMainArgValue<Enum>> {
        std::filesystem::path programPath{};
        std::vector<std::string> freeArgs{};
    };

    template<class Enum,std::size_t Size>
    Invocation<Enum> parseArgs(std::span<const std::string_view>& args, const std::array<BMainArg<Enum>,Size>& argSpec){
        using ArgListIterator = std::array<BMainArg<Enum>,Size>::const_iterator;
        bool doubleDash{false};

        Invocation<Enum> invocation{};
        invocation.programPath = args.front();
        auto argList = args.subspan<1>();
        for (size_t idx = 0; idx < argList.size(); ++idx) {
            ArgListIterator valuedOption = argSpec.end();
            auto argString = argList[idx];
            if (!argString.empty() && argString[0] == '-') {
                if (argString.size() > 1 && argString[1] == '-') {
                    ArgListIterator argItem = std::find_if(argSpec.begin(), argSpec.end(),
                        [&argSpec,argString](auto& arg) -> bool {
                            return arg.longArg == argString.substr(2);
                        });
                    if (argItem != argSpec.end()) {
                        if (argItem->takesValue != ArgType::NoValue)
                            valuedOption = argItem;
                        else
                            invocation.emplace_back(argItem->argIdx);
                    } else {
                        throw std::runtime_error("Command line option not found.");
                    }
                } else {
                    bool valueUsed{false};
                    for (auto argChar : argString.substr(1)) {
                        ArgListIterator argItem = std::find_if(argSpec.begin(), argSpec.end(),
                                     [&argSpec, argChar](auto &arg) -> bool {
                                         return arg.shortArg == argChar;
                                     });
                        if (argItem != argSpec.end()) {
                            if (argItem->takesValue != ArgType::NoValue) {
                                if (!valueUsed) {
                                    valuedOption = argItem;
                                    valueUsed = true;
                                } else {
                                    throw std::runtime_error(ysh::StringComposite("In option '", argString,
                                                                                  "' more than one option takes an argument."));
                                }
                            } else {
                                invocation.emplace_back(argItem->argIdx);
                            }
                        } else {
                            throw std::runtime_error(ysh::StringComposite("Command line option '", argChar, "' not found."));
                        }
                    }
                }

                if (valuedOption != argSpec.end()) {
                    if (argList.size() > idx+1) {
                        ++idx;
                        invocation.emplace_back(valuedOption->argIdx, std::string(argList[idx]));
                    } else
                        throw std::runtime_error(ysh::StringComposite("Command line option '", valuedOption->longArg,
                                      "' takes a value but none is provided."));
                }
            } else {
                invocation.freeArgs.emplace_back(std::string(argList[idx]));
            }
        }
        return invocation;
    }

} // better_main

int main(int argc, char const * const * argv);

#endif //VE3YSH_UTIL_BMAIN_H

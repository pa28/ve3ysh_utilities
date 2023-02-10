//
// Created by richard on 09/02/23.
//

#include "BetterMain/BMain.h"
#include <iostream>
#include <algorithm>

enum class ArgIdx : size_t {
    Help,
    ConfigFile,
    Input,
    Output,
    Verbose,
    ArgCount
};

namespace better_main {
    static constexpr std::array<better_main::BMainArg<ArgIdx>,static_cast<size_t>(ArgIdx::ArgCount)> ProgramArgs = {{
        { ArgIdx::Help, ArgType::Help, 'h', "help", "Display program or argument help"},
        { ArgIdx::ConfigFile, ArgType::Path, 'c', "config", "Specify the configuration file."},
        { ArgIdx::Input, ArgType::Path, 'i', "input", "Specify the input file."},
        { ArgIdx::Output, ArgType::Path, 'o', "output", "Specify the output file."},
        { ArgIdx::Verbose, ArgType::NoValue, 'v', "verbose", "Run in verbose mode."}
    }};

    [[nodiscard]] int start(std::span<const std::string_view> args) noexcept {
        auto invocation = parseArgs(args, ProgramArgs);
        for (const auto& arg : invocation) {
            if (auto option = std::ranges::find_if(ProgramArgs,
                   [&arg](auto opt) { return opt.argIdx == arg.argIdx; }); option != ProgramArgs.end()) {
                std::cout << '\t' << option->longArg << " '" << arg.value << "'\n";
            }
        }

        for (const auto& arg : invocation.freeArgs) {
            std::cout << '\t' << arg << '\n';
        }

        auto verbose = occurrenceCount(ArgIdx::Verbose, invocation);
        std::cout << verbose << '\n';
        return 0;
    }
}
//
// Created by richard on 09/02/23.
//

#include "BetterMain/BMain.h"
#include <iostream>
#include <algorithm>
#include <fstream>

enum class ArgIdx : size_t {
    FreeArg,
    Help,
    Output,
    ExecName,
    User,
    Group,
    ArgCount
};

namespace better_main {
    static constexpr std::array<better_main::BMainArg<ArgIdx>,static_cast<size_t>(ArgIdx::ArgCount)> ProgramArgs = {{
        { ArgIdx::FreeArg, ArgType::FreeArg, '\0', "", "", ""},
        { ArgIdx::Help, ArgType::Help, 'h', "help", "Display program or argument help", ""},
        { ArgIdx::ExecName, ArgType::String, 'e', "execName", "Specify the executable to generate for.", ""},
        { ArgIdx::Output, ArgType::Path, 'o', "output", "Specify the output file.", ""},
        { ArgIdx::User, ArgType::User, 'u', "user" "", "" },
        { ArgIdx::Group, ArgType::Group, 'g', "group", "", ""}
    }};

    [[nodiscard]] int start(std::span<const std::string_view> args) noexcept {
        auto invocation = parseArgs(args, ProgramArgs);
        for (const auto& arg : invocation) {
            if (arg.argType == ArgType::FreeArg)
                std::cout << "\tFree arg: '" << arg.value << "'\n";
            else if (auto option = std::ranges::find_if(ProgramArgs,
                   [&arg](auto opt) { return opt.argIdx == arg.argIdx; }); option != ProgramArgs.end()) {
                std::cout << '\t' << option->longArg << " '" << arg.value << "'\n";
            }
        }

        if (auto execName = findArgument(invocation, ArgIdx::ExecName); execName.has_value()) {
            if (auto output = findArgument(invocation, ArgIdx::Output); output.has_value()) {
                std::ofstream fStr{output.value().value};
                generateCompletionFile(execName.value().value, ProgramArgs, fStr);
                fStr.close();
            } else {
                std::cerr << "Output file required.\n";
                return 1;
            }
        } else {
            std::cerr << "Executable name is required.\n";
            return 2;
        }

        return 0;
    }
}
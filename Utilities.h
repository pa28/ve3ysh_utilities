
#include <sstream>

/**
* @brief Composite a pack of arguments that are streamable to a string.
* @tparam Arg First argument type
* @tparam Args Rest of the argument types
* @param arg First argument
* @param args Rest of the arguments
* @return The composited string
*/
template<typename Arg, typename... Args>
inline std::string StringCompositor(Arg &&arg, Args &&... args) {
    std::stringstream out;
    out << std::forward<Arg>(arg);
    ((out << std::forward<Args>(args)), ...);
    return out.str();
}


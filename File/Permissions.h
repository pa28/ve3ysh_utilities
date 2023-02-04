//
// Created by richard on 30/12/22.
//

/*
 * Permissions.h Created by Richard Buckley (C) 30/12/22
 */

#include <filesystem>

#ifndef YSH_PERMISSIONS_H
#define YSH_PERMISSIONS_H

namespace ysh {

/**
 * @class Permissions
 */
    class Permissions {
    private:
        std::filesystem::path filePath{};
        std::filesystem::perms filePerms{};
        std::filesystem::file_status fileStatus{};

        void evaluate();

        bool fileExists{false}; ///< File exists
        bool known{false};      ///< File permissions are known
        unsigned int procPerms{0};  ///< The aggregated permissions the process holds on the file.

    public:
        static const unsigned int READ{04};
        static const unsigned int WRITE{02};
        static const unsigned int EXEC{01};

        Permissions() = default;

        explicit Permissions(std::filesystem::path filePath) : filePath(std::move(filePath)) {
            evaluate();
        }

        /**
         * @brief Test validity of the Permission object.
         * @return True if the file exists and permissions are known.
         */
        explicit operator bool () const { return fileExists && known; }

        /**
         * @brief Test if the process has read permission on the file.
         * @return true or false
         */
        [[maybe_unused]] [[nodiscard]] bool read() const { return fileExists && known && ((procPerms & READ) != 0); }

        /**
         * @brief Test if the process has write permission on the file.
         * @return true of false
         */
        [[maybe_unused]] [[nodiscard]] bool write() const { return fileExists && known && ((procPerms & WRITE) != 0); }

        /**
         * @brief Test if the process has execute permission on the file.
         * @return true or false
         */
        [[maybe_unused]] [[nodiscard]] bool exec() const { return fileExists && known && ((procPerms & EXEC) != 0); }

        /**
         * @brief Test if the process has all indicated permissions on the file.
         * @param maskValue a bitwise OR of READ, WRITE, EXEC class constants.
         * @return true if the process has all requested permissions.
         */
        [[maybe_unused]] [[nodiscard]] bool mask(unsigned int maskValue) const {
            return fileExists && known && ((procPerms & maskValue) == maskValue);
        }

        /**
         * @brief Set a new file path and evaluate permissions.
         * @param path the new file path.
         * @return true if the file exists and permissions are known.
         */
        [[maybe_unused]] bool setFile(std::filesystem::path path) {
            filePath = std::move(path);
            evaluate();
            return fileExists && known;
        }
    };

} // ysh

#endif //YSH_PERMISSIONS_H

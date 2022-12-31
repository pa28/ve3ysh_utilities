//
// Created by richard on 30/12/22.
//

/*
 * Permissions.cpp Created by Richard Buckley (C) 30/12/22
 */

#include <unistd.h>
#include <sys/stat.h>
#include <iostream>
#include "Permissions.h"

namespace ysh {

    using namespace std::filesystem;
    void Permissions::evaluate() {
        fileExists = known = false;
        procPerms = 0;
        auto euid = geteuid();
        auto egid = getegid();
        if (!filePath.empty()) {
            if (fileExists = exists(filePath); fileExists) {
                fileStatus = status(filePath);
                filePerms = fileStatus.permissions();
                struct stat statBuf{};
                if (stat(filePath.c_str(), &statBuf)) {
                    std::cerr << "Can not stat " << filePath.string() << '\n';
                } else {
                    if (euid == statBuf.st_uid) {
                        procPerms |= ((filePerms & perms::owner_read) != perms::none) ? READ : 0;
                        procPerms |= ((filePerms & perms::owner_write) != perms::none) ? WRITE : 0;
                        procPerms |= ((filePerms & perms::owner_exec) != perms::none) ? EXEC : 0;
                    }

                    if (egid == statBuf.st_gid) {
                        procPerms |= ((filePerms & perms::group_read) != perms::none) ? READ : 0;
                        procPerms |= ((filePerms & perms::group_write) != perms::none) ? WRITE : 0;
                        procPerms |= ((filePerms & perms::group_exec) != perms::none) ? EXEC : 0;
                    }

                    procPerms |= ((filePerms & perms::others_read) != perms::none) ? READ : 0;
                    procPerms |= ((filePerms & perms::others_write) != perms::none) ? WRITE : 0;
                    procPerms |= ((filePerms & perms::others_exec) != perms::none) ? EXEC : 0;

                    known = true;
                }
            }
        }
    }
} // ysh

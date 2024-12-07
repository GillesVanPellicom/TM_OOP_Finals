// ╔══════════════════════════════════════════════════════════════════════════════════╗
// ║ Name         : User.h                                                      ║
// ║ Description  : Lorem ipsum dolor sit amet                                        ║
// ║                Lorem ipsum dolor sit amet                                        ║
// ║ Author(s)    : "Gilles Van pellicom" <r0997008@student.thomasmore.be>            ║
// ║ Date         : 2024/12/07                                                        ║
// ╚══════════════════════════════════════════════════════════════════════════════════╝

#ifndef USER_H
#define USER_H
#include <iostream>
#include <ostream>
#include <string>

enum UserPermissionLevel {
  ADMIN,
  EMPLOYEE
};

class User {
  std::string userName;
  UserPermissionLevel permissionLevel;

  public:
    User(std::string user_name, const UserPermissionLevel permission_level)
      : userName(std::move(user_name)),
        permissionLevel(permission_level) {
    }

    //userName
    [[nodiscard]] std::string getUserName() const {
      return userName;
    }
    void setUserName(const std::string& user_name) {
      userName = user_name;
    }

    //permissionLevel
    [[nodiscard]] UserPermissionLevel getPermissionLevel() const {
      return permissionLevel;
    }
    void setPermissionLevel(const UserPermissionLevel permission_level) {
      permissionLevel = permission_level;
    }
};


#endif //USER_H

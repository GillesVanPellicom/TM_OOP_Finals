// ╔══════════════════════════════════════════════════════════════════════════════════╗
// ║ Name         : User.h                                                            ║
// ║ Description  : Definition of the class User                                      ║
// ║ Author(s)    : "Gilles Van pellicom" <r0997008@student.thomasmore.be>            ║
// ║ Date         : 2024/12/07                                                        ║
// ╚══════════════════════════════════════════════════════════════════════════════════╝

#ifndef USER_H
#define USER_H

// STD
#include <ostream>
#include <string>


enum UserPermissionLevel {
  ADMIN,
  EMPLOYEE
};


/**
 * @class User
 * @brief Represents system a user with a username and a permission level.
 */
class User {
  private:
  // ╔════════════════════════════════════════╗
  // ║              Attributes                ║
  // ╚════════════════════════════════════════╝

  std::string userName;
  UserPermissionLevel permissionLevel;


  // ╔════════════════════════════════════════╗
  // ║             Constructors               ║
  // ╚════════════════════════════════════════╝

  public:
    User(std::string user_name, const UserPermissionLevel permission_level)
      : userName(std::move(user_name)),
        permissionLevel(permission_level) {
    }


  // ╔════════════════════════════════════════╗
  // ║           Getters & Setters            ║
  // ╚════════════════════════════════════════╝

    [[nodiscard]] std::string getUserName() const {
      return userName;
    }
    void setUserName(const std::string& user_name) {
      userName = user_name;
    }


    [[nodiscard]] UserPermissionLevel getPermissionLevel() const {
      return permissionLevel;
    }
    void setPermissionLevel(const UserPermissionLevel permission_level) {
      permissionLevel = permission_level;
    }
};


#endif //USER_H

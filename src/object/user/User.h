// ╔══════════════════════════════════════════════════════════════════════════════════╗
// ║ Name         : User.h                                                            ║
// ║ Description  : Definition of the class User                                      ║
// ║ Author(s)    : "Gilles Van pellicom" <r0997008@student.thomasmore.be>            ║
// ║ Date         : 2024/12/07                                                        ║
// ╚══════════════════════════════════════════════════════════════════════════════════╝

#ifndef USER_H
#define USER_H

// STD
#include <json.hpp>
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
    [[nodiscard]] std::string getPermissionLevelAsString() const {
      return permissionLevel == ADMIN ? "admin" : "employee";
    }
    void setPermissionLevel(const UserPermissionLevel permission_level) {
      permissionLevel = permission_level;
    }
    void setPermissionLevelAsString(const std::string& permission_level) {
      permissionLevel = permission_level == "admin" ? ADMIN : EMPLOYEE;
    }


    // ╔════════════════════════════════════════╗
    // ║            Public Methods              ║
    // ╚════════════════════════════════════════╝

    /**
      * @brief Serializes this specific object, used in recursive serialization.
      * @return Object as JSON
      */
    [[nodiscard]] nlohmann::json serialize();


    /**
      * @brief Deserializes this specific object, used in recursive deserialization.
      */
    void deserialize(const nlohmann::json& j);
};


#endif //USER_H

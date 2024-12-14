// ╔══════════════════════════════════════════════════════════════════════════════════╗
// ║ Name         : User.cpp                                                          ║
// ║ Description  : Implementation of the class User                                  ║
// ║ Author(s)    : "Gilles Van pellicom" <r0997008@student.thomasmore.be>            ║
// ║ Date         : 2024/12/07                                                        ║
// ╚══════════════════════════════════════════════════════════════════════════════════╝

#include "User.h"

nlohmann::json User::serialize() {
  nlohmann::json j = {
    {"userName", this->getUserName()},
    {"permissionLevel", this->getPermissionLevelAsString()}
  };
  return j;
}


void User::deserialize(const nlohmann::json& j) {
  this->setUserName(j.at("userName").get<std::string>());
  this->setPermissionLevelAsString(j.at("permissionLevel").get<std::string>());

}

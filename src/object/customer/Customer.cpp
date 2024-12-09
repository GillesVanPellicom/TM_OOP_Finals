// ╔══════════════════════════════════════════════════════════════════════════════════╗
// ║ Name         : Customer.cpp                                                      ║
// ║ Description  : Lorem ipsum dolor sit amet                                        ║
// ║                Lorem ipsum dolor sit amet                                        ║
// ║ Author(s)    : "Gilles Van pellicom" <r0997008@student.thomasmore.be>            ║
// ║ Date         : 2024/12/07                                                        ║                
// ║ Version      : 1.0                                                               ║
// ║ License      : GPL-3.0                                                           ║
// ╚══════════════════════════════════════════════════════════════════════════════════╝

#include "Customer.h"

nlohmann::json Customer::serialize() {
  nlohmann::json j = {
    {"firstName", this->getFirstName()},
    {"lastName", this->getLastName()},
    {"address", this->getAddress()},
    {"business", this->isBusinessCustomer()}
  };
  return j;
}

void Customer::deserialize(const nlohmann::json& j) {
  this->setFirstName(j.at("firstName").get<std::string>());
  this->setLastName(j.at("lastName").get<std::string>());
  this->setAddress(j.at("address").get<std::string>());
  this->setBusinessCustomer(j.at("business").get<bool>());
}
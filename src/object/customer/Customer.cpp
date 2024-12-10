// ╔══════════════════════════════════════════════════════════════════════════════════╗
// ║ Name         : Customer.cpp                                                      ║
// ║ Description  : Implementation of class Customer                                  ║
// ║ Author(s)    : "Gilles Van pellicom" <r0997008@student.thomasmore.be>            ║
// ║ Date         : 2024/12/07                                                        ║
// ╚══════════════════════════════════════════════════════════════════════════════════╝

#include "Customer.h"

nlohmann::json Customer::serialize() const {
  nlohmann::json j = {
    {"firstName", this->getFirstName()},
    {"lastName", this->getLastName()},
    {"address", this->getAddress()},
    {"business", this->isBusinessCustomer()},
    {"UUID", this->getUUID()}
  };
  return j;
}

void Customer::deserialize(const nlohmann::json& j) {
  this->setFirstName(j.at("firstName").get<std::string>());
  this->setLastName(j.at("lastName").get<std::string>());
  this->setAddress(j.at("address").get<std::string>());
  this->setBusinessCustomer(j.at("business").get<bool>());
  this->setUUID(j.at("UUID").get<std::string>());
}

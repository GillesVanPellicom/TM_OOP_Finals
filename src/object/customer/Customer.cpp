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
    {"UUID", this->getUUID()},
    {"CompanyUUID", this->getCompanyUUID()}
  };
  return j;
}


void Customer::deserialize(const nlohmann::json& j) {
  this->setFirstName(j.at("firstName").get<std::string>());
  this->setLastName(j.at("lastName").get<std::string>());
  this->setAddress(j.at("address").get<std::string>());
  this->setBusinessCustomer(j.at("business").get<bool>());
  this->setUUID(j.at("UUID").get<std::string>());
  this->setCompanyUUID(j.at("CompanyUUID").get<std::string>());
}


std::string Customer::buildCustomerInfo(const std::shared_ptr<Company>& c) const {
  std::ostringstream info;

  const std::string businessType = this->isBusinessCustomer() ? "Business" : "Individual";

  info
      << "Name      : " << this->getFirstName()
      << "\nSurname   : " << this->getLastName()
      << "\nAddress   : " << this->getAddress()
      << "\nType      : " << businessType;

  if (this->isBusinessCustomer()) {
    info
        << "\nVAT       : " << c->getVat()
        << "\nV. Discnt.: " << c->getVolumeDiscount() << "%";
  }

  return info.str();
}

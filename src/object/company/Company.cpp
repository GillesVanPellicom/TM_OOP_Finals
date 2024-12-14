// ╔══════════════════════════════════════════════════════════════════════════════════╗
// ║ Name         : Company.h                                                         ║
// ║ Description  : Implementation of class Company                                   ║
// ║ Author(s)    : "Gilles Van pellicom" <r0997008@student.thomasmore.be>            ║
// ║ Date         : 2024/12/07                                                        ║
// ╚══════════════════════════════════════════════════════════════════════════════════╝

#include "Company.h"

nlohmann::json Company::serialize() const {
  nlohmann::json j = {
    {"UUID", this->getUUID()},
    {"vat", this->getVat()},
    {"volumeDiscount", this->getVolumeDiscount()}
  };
  return j;
}


void Company::deserialize(const nlohmann::json& j) {
  this->setUUID(j.at("UUID").get<std::string>());
  this->setVat(j.at("vat").get<std::string>());
  this->setVolumeDiscount(j.at("volumeDiscount").get<u_int32_t>());
}

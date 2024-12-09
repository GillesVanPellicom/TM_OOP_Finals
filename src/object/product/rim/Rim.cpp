// ╔══════════════════════════════════════════════════════════════════════════════════╗
// ║ Name         : Rim.cpp                                                      ║
// ║ Description  : Lorem ipsum dolor sit amet                                        ║
// ║                Lorem ipsum dolor sit amet                                        ║
// ║ Author(s)    : "Gilles Van pellicom" <r0997008@student.thomasmore.be>            ║
// ║ Date         : 2024/12/07                                                        ║                
// ║ Version      : 1.0                                                               ║
// ║ License      : GPL-3.0                                                           ║
// ╚══════════════════════════════════════════════════════════════════════════════════╝

#include "Rim.h"

nlohmann::json Rim::serialize() {
  nlohmann::json j = {
    {"type", this->getTypeAsString()},
    {"name", this->getName()},
    {"manufacturer", this->getManufacturer()},
    {"diameter", this->getDiameter()},
    {"stockCount", this->getStockCount()},
    {"priceIndividual", this->getPriceIndividual()},
    {"priceBusiness", this->getPriceBusiness()},
    {"width", this->getWidth()},
    {"color", this->getColor()},
    {"material", this->getMaterialAsString()},
    {"UUID", this->getUUID()}
  };
  return j;
}

void Rim::deserialize(const nlohmann::json& j) {
  this->setTypeAsString(j.at("type").get<std::string>());
  this->setName(j.at("name").get<std::string>());
  this->setManufacturer(j.at("manufacturer").get<std::string>());
  this->setDiameter(j.at("diameter").get<std::uint32_t>());
  this->setStockCount(j.at("stockCount").get<std::uint32_t>());
  this->setPriceIndividual(j.at("priceIndividual").get<std::uint64_t>());
  this->setPriceBusiness(j.at("priceBusiness").get<std::uint64_t>());
  this->setWidth(j.at("width").get<std::uint32_t>());
  this->setColor(j.at("color").get<std::string>());
  this->setMaterialAsString(j.at("material").get<std::string>());
  this->setUUID(j.at("UUID").get<std::string>());
}
// ╔══════════════════════════════════════════════════════════════════════════════════╗
// ║ Name         : Tire.cpp                                                          ║
// ║ Description  : Implementation of the class Tire                                  ║
// ║ Inherit(s)   : Product.h                                                         ║
// ║ Author(s)    : "Gilles Van pellicom" <r0997008@student.thomasmore.be>            ║
// ║ Date         : 2024/12/07                                                        ║
// ╚══════════════════════════════════════════════════════════════════════════════════╝

#include "Tire.h"

nlohmann::json Tire::serialize() {
  nlohmann::json j = {
    {"type", this->getTypeAsString()},
    {"name", this->getName()},
    {"manufacturer", this->getManufacturer()},
    {"diameter", this->getDiameter()},
    {"stockCount", this->getStockCount()},
    {"priceIndividual", this->getPriceIndividual()},
    {"priceBusiness", this->getPriceBusiness()},
    {"width", this->getWidth()},
    {"height", this->getHeight()},
    {"speedIndex", this->getSpeedIndex()},
    {"UUID", this->getUUID()}
  };
  return j;
}


void Tire::deserialize(const nlohmann::json& j) {
  this->setTypeAsString(j.at("type").get<std::string>());
  this->setName(j.at("name").get<std::string>());
  this->setManufacturer(j.at("manufacturer").get<std::string>());
  this->setDiameter(j.at("diameter").get<std::uint32_t>());
  this->setStockCount(j.at("stockCount").get<std::uint32_t>());
  this->setPriceIndividual(j.at("priceIndividual").get<std::uint64_t>());
  this->setPriceBusiness(j.at("priceBusiness").get<std::uint64_t>());
  this->setWidth(j.at("width").get<std::uint32_t>());
  this->setHeight(j.at("height").get<std::uint32_t>());
  this->setSpeedIndex(j.at("speedIndex").get<char>());
  this->setUUID(j.at("UUID").get<std::string>());
}


std::string Tire::buildProductInfo() {
  std::ostringstream info;
  info << "Name               : " << getName()
      << "\nManufacturer       : " << getManufacturer()
      << "\nType               : " << getTypeAsString()
      << "\nDiameter           : " << std::to_string(this->getDiameter()) << " (inch)"
      << "\nWidth              : " << std::to_string(getWidth()) << " (mm)"
      << "\nHeight             : " << std::to_string(getHeight()) << " (mm)"
      << "\nSpeed Index        : " << std::string(1, getSpeedIndex())
      << "\nStock Count        : " << std::to_string(this->getStockCount())
      << "\nPrice (Individual) : " << convertCentsToReadable(this->getPriceIndividual())
      << "\nPrice (Business)   : " << convertCentsToReadable(this->getPriceBusiness());

  return info.str();
}

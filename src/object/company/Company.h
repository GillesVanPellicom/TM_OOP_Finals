// ╔══════════════════════════════════════════════════════════════════════════════════╗
// ║ Name         : Company.h                                                         ║
// ║ Description  : Definition of class Company                                       ║
// ║ Author(s)    : "Gilles Van pellicom" <r0997008@student.thomasmore.be>            ║
// ║ Date         : 2024/12/07                                                        ║
// ╚══════════════════════════════════════════════════════════════════════════════════╝

#ifndef COMPANY_H
#define COMPANY_H

// STD
#include <json.hpp>
#include <string>

// Local
#include "../../util/IdGen.hpp"


class Company {
  private:
    // ╔════════════════════════════════════════╗
    // ║              Attributes                ║
    // ╚════════════════════════════════════════╝

    std::string vat;
    uint32_t volumeDiscount;
    UUIDGen::UUID uuid = UUIDGen::generateUUID();

  public:
    // ╔════════════════════════════════════════╗
    // ║             Constructors               ║
    // ╚════════════════════════════════════════╝

    Company(std::string vat, const uint32_t volume_discount)
      : vat(std::move(vat)),
        volumeDiscount(volume_discount) {
    }


    explicit Company(const nlohmann::json& j) {
      deserialize(j);
    }


    // ╔════════════════════════════════════════╗
    // ║              Destructors               ║
    // ╚════════════════════════════════════════╝

    ~Company() = default;


    // ╔════════════════════════════════════════╗
    // ║           Getters & Setters            ║
    // ╚════════════════════════════════════════╝

    [[nodiscard]] std::string getVat() const {
      return vat;
    }
    void setVat(const std::string& vat) {
      this->vat = vat;
    }


    [[nodiscard]] uint32_t getVolumeDiscount() const {
      return volumeDiscount;
    }
    void setVolumeDiscount(const uint32_t volume_discount) {
      volumeDiscount = volume_discount;
    }

    [[nodiscard]] UUIDGen::UUID getUUID() const {
      return uuid;
    }
    void setUUID(const UUIDGen::UUID& uuid) {
      this->uuid = uuid;
    }


    // ╔════════════════════════════════════════╗
    // ║            Public Methods              ║
    // ╚════════════════════════════════════════╝

    /**
      * @brief Serializes class to JSON.
      * Used for recursive serialization
      * @return class serialized as JSON object
      */
    [[nodiscard]] nlohmann::json serialize() const;


    /**
      * @brief Deserializes class from JSON
      * Used for recursive deserialization
      * @param j JSON object from which to de-serialize
      */
    void deserialize(const nlohmann::json& j);
};


#endif //COMPANY_H

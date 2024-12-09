// ╔══════════════════════════════════════════════════════════════════════════════════╗
// ║ Name         : Customer.h                                                      ║
// ║ Description  : Lorem ipsum dolor sit amet                                        ║
// ║                Lorem ipsum dolor sit amet                                        ║
// ║ Author(s)    : "Gilles Van pellicom" <r0997008@student.thomasmore.be>            ║
// ║ Date         : 2024/12/07                                                        ║
// ╚══════════════════════════════════════════════════════════════════════════════════╝

#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <json.hpp>
#include <utility>
#include <vector>

#include "../../util/IdGen.hpp"


class Customer {
  std::string firstName;
  std::string lastName;
  std::string address;
  bool businessCustomer = false;

  UUIDGen::UUID uuid = UUIDGen::generateUUID();

  public:
    // ╔════════════════════════════════════════╗
    // ║             Constructors               ║
    // ╚════════════════════════════════════════╝

    Customer(std::string first_name,
             std::string last_name,
             std::string address,
             bool business_customer)
      : firstName(std::move(first_name)),
        lastName(std::move(last_name)),
        address(std::move(address)),
        businessCustomer(business_customer) {
    }

    explicit Customer(const nlohmann::json& j) {
      deserialize(j);
    }

    // ╔════════════════════════════════════════╗
    // ║           Getters & Setters            ║
    // ╚════════════════════════════════════════╝

    // firstName
    [[nodiscard]] std::string getFirstName() const {
      return firstName;
    }
    void setFirstName(std::string first_name) {
      firstName = std::move(first_name);
    }

    // lastName
    [[nodiscard]] std::string getLastName() const {
      return lastName;
    }
    void setLastName(std::string last_name) {
      this->lastName = std::move(last_name);
    }

    // address
    [[nodiscard]] std::string getAddress() const {
      return address;
    }
    void setAddress(std::string address) {
      this->address = std::move(address);
    }

    // businessCustomer
    [[nodiscard]] bool isBusinessCustomer() const {
      return businessCustomer;
    }
    void setBusinessCustomer(const bool is_business_customer) {
      businessCustomer = is_business_customer;
    }

    // UUID
    [[nodiscard]] UUIDGen::UUID getUUID() const {
      return uuid;
    }
    void setUUID(const UUIDGen::UUID& uuid) {
      this->uuid = uuid;
    }

    // ╔════════════════════════════════════════╗
    // ║               functions                ║
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


#endif //CUSTOMER_H

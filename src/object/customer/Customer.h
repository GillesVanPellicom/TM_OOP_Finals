// ╔══════════════════════════════════════════════════════════════════════════════════╗
// ║ Name         : Customer.h                                                        ║
// ║ Description  : Definition of class Customer                                      ║
// ║ Author(s)    : "Gilles Van pellicom" <r0997008@student.thomasmore.be>            ║
// ║ Date         : 2024/12/07                                                        ║
// ╚══════════════════════════════════════════════════════════════════════════════════╝

#ifndef CUSTOMER_H
#define CUSTOMER_H

// STD
#include <json.hpp>
#include <utility>

// Local
#include "../../util/IdGen.hpp"


/**
 * @brief Class representing a customer in the system.
 *
 * The `Customer` class holds information about a customer, including their name, address,
 * business status, and a unique identifier (UUID). It supports serialization and deserialization
 * for saving and loading customer data, as well as the ability to modify and retrieve customer details.
 *
 * @note A `Customer` can represent both individual and business customers. The business status
 *       is indicated by the `businessCustomer` attribute.
 *
 * @see UUIDGen
 */
class Customer {
  private:
    // ╔════════════════════════════════════════╗
    // ║              Attributes                ║
    // ╚════════════════════════════════════════╝

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

    [[nodiscard]] std::string getFirstName() const {
      return firstName;
    }
    void setFirstName(std::string first_name) {
      firstName = std::move(first_name);
    }


    [[nodiscard]] std::string getLastName() const {
      return lastName;
    }
    void setLastName(std::string last_name) {
      this->lastName = std::move(last_name);
    }


    [[nodiscard]] std::string getAddress() const {
      return address;
    }
    void setAddress(std::string address) {
      this->address = std::move(address);
    }


    [[nodiscard]] bool isBusinessCustomer() const {
      return businessCustomer;
    }
    void setBusinessCustomer(const bool is_business_customer) {
      businessCustomer = is_business_customer;
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


#endif //CUSTOMER_H

// ╔══════════════════════════════════════════════════════════════════════════════════╗
// ║ Name         : Customer.h                                                      ║
// ║ Description  : Lorem ipsum dolor sit amet                                        ║
// ║                Lorem ipsum dolor sit amet                                        ║
// ║ Author(s)    : "Gilles Van pellicom" <r0997008@student.thomasmore.be>            ║
// ║ Date         : 2024/12/07                                                        ║
// ╚══════════════════════════════════════════════════════════════════════════════════╝

#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <vector>


class Customer {
  std::string firstName;
  std::string lastName;
  std::string address;
  bool businessCustomer = false;

  public:
    // ╔════════════════════════════════════════╗
    // ║             Constructors               ║
    // ╚════════════════════════════════════════╝

    Customer(std::string first_name,
             std::string last_name,
             std::string address)
      : firstName(std::move(first_name)),
        lastName(std::move(last_name)),
        address(std::move(address)) {
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
};


#endif //CUSTOMER_H

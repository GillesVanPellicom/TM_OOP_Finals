// ╔══════════════════════════════════════════════════════════════════════════════════╗
// ║ Name         : Invoice.h                                                      ║
// ║ Description  : Lorem ipsum dolor sit amet                                        ║
// ║                Lorem ipsum dolor sit amet                                        ║
// ║ Author(s)    : "Gilles Van pellicom" <r0997008@student.thomasmore.be>            ║
// ║ Date         : 2024/12/09                                                        ║                
// ║ Version      : 1.0                                                               ║
// ║ License      : GPL-3.0                                                           ║
// ╚══════════════════════════════════════════════════════════════════════════════════╝

#ifndef INVOICE_H
#define INVOICE_H

#include "../customer/Customer.h"
#include "../../util/IdGen.hpp"
#include "../../util/DateGen.hpp"
#include "../../object/product/Product.h"
#include "../../object/product/tire/Tire.h"
#include "../../object/product/rim/Rim.h"

class Invoice {
  std::string invoiceName;

  UUIDGen::UUID customerId; // Can be safely dangled, no UUID references used after object construction
  std::string firstName;
  std::string lastName;
  std::string address;
  bool business = false;
  // sub purchases for this invoice. Use UUID since pointers can't be serialized/deserialized
  // productID, qty, productType, name, price
  std::vector<std::tuple<UUIDGen::UUID, uint32_t, ProductType, std::string, std::uint64_t> > purchaseList;
  std::uint64_t noDiscountPrice = 0;
  std::uint64_t finalPrice = 0;
  std::uint64_t discountRate = 0;

  void calculatePrice();

  public:
    explicit Invoice(const std::shared_ptr<Customer>& c)
      : invoiceName(c->getFirstName() + " " + getCurrentDateTime()),
        customerId(c->getUUID()),
        firstName(c->getFirstName()),
        lastName(c->getLastName()),
        address(c->getAddress()),
        business(c->isBusinessCustomer()) {
    }

    void addPurchase(const std::shared_ptr<Product>& p, uint32_t count);

    [[nodiscard]] std::string buildInvoiceInfo() const;


    [[nodiscard]] UUIDGen::UUID getUUID() const {
      return customerId;
    }
    void setUUID(const UUIDGen::UUID& UUID) {
      customerId = UUID;
    }


    [[nodiscard]] std::string getFirstName() const {
      return firstName;
    }
    void setFirstName(const std::string& first_name) {
      firstName = first_name;
    }


    [[nodiscard]] std::string getLastName() const {
      return lastName;
    }
    void setLastName(const std::string& last_name) {
      lastName = last_name;
    }


    [[nodiscard]] std::string getAddress() const {
      return address;
    }
    void setAddress(const std::string& address) {
      this->address = address;
    }


    [[nodiscard]] bool isBusiness() const {
      return business;
    }
    void setIsBusiness(const bool business) {
      this->business = business;
    }


    [[nodiscard]] std::uint64_t getNoDiscountPrice() const {
      return noDiscountPrice;
    }
    void setNoDiscountPrice(const std::uint64_t no_discount_price) {
      noDiscountPrice = no_discount_price;
    }


    [[nodiscard]] std::uint64_t getFinalPrice() const {
      return finalPrice;
    }
    void setFinalPrice(const std::uint64_t final_price) {
      finalPrice = final_price;
    }


    [[nodiscard]] std::uint64_t getDiscountRate() const {
      return discountRate;
    }
    void setDiscountRate(const std::uint64_t discount_rate) {
      discountRate = discount_rate;
    }


    [[nodiscard]] std::string getInvoiceName() const {
      return invoiceName;
    }
    void setInvoiceName(const std::string& invoice_name) {
      invoiceName = invoice_name;
    }
};


#endif //INVOICE_H

// ╔══════════════════════════════════════════════════════════════════════════════════╗
// ║ Name         : Invoice.h                                                         ║
// ║ Description  : Definition of class Invoice                                       ║
// ║ Author(s)    : "Gilles Van pellicom" <r0997008@student.thomasmore.be>            ║
// ║ Date         : 2024/12/09                                                        ║
// ╚══════════════════════════════════════════════════════════════════════════════════╝

#ifndef INVOICE_H
#define INVOICE_H

// Local
#include "../customer/Customer.h"
#include "../../util/DateGen.hpp"
#include "../../object/product/Product.h"


/**
 * @brief Class representing an invoice for customer purchases.
 *
 * The `Invoice` class holds all the necessary information for a customer's invoice,
 * including customer details, purchased products, pricing, and discount information.
 * It provides methods for serializing, deserializing, and managing invoice data.
 *
 * @note This class assumes that relevant customer and product data are already available
 *       before the invoice is created.
 *
 * @see Customer
 * @see Product
 */
class Invoice {
  private:
    // ╔════════════════════════════════════════╗
    // ║              Attributes                ║
    // ╚════════════════════════════════════════╝

    std::string invoiceName;
    UUIDGen::UUID customerId; // Can be safely dangled, no UUID references used after object construction
    std::string firstName;
    std::string lastName;
    std::string address;
    bool business = false;
    // sub purchases for this invoice Use UUID since pointers can't be serialized/deserialized
    // productID, qty, productType, name, price
    std::vector<std::tuple<UUIDGen::UUID, uint32_t, ProductType, std::string, std::uint64_t> > purchaseList;
    std::uint64_t noDiscountPrice = 0;
    std::uint64_t finalPrice = 0;
    std::uint64_t discountRate = 0;
    std::uint64_t businessDiscount = 0;


    // ╔════════════════════════════════════════╗
    // ║           Private Methods              ║
    // ╚════════════════════════════════════════╝

    /**
     * @brief Used internally to update price and discount attributes.
     */
    void calculatePrice();

  public:
    // ╔════════════════════════════════════════╗
    // ║             Constructors               ║
    // ╚════════════════════════════════════════╝

    explicit Invoice(const std::shared_ptr<Customer>& c, const uint32_t businessDiscount)
      : invoiceName(c->getFirstName() + " " + c->getLastName() + " " + getCurrentDateTime()),
        customerId(c->getUUID()),
        firstName(c->getFirstName()),
        lastName(c->getLastName()),
        address(c->getAddress()),
        business(c->isBusinessCustomer()),
        businessDiscount(businessDiscount) {
    }


    explicit Invoice(const nlohmann::json& j) {
      deserialize(j);
    }


    // ╔════════════════════════════════════════╗
    // ║              Destructors               ║
    // ╚════════════════════════════════════════╝

    ~Invoice() = default;


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
     * @brief Deserializes class from JSON.
     * Used for recursive deserialization
     * @param j JSON object from which to de-serialize
     */
    void deserialize(const nlohmann::json& j);


    /**
     * @brief Adds a purchase to the invoice?
     * @param p product to be added
     * @param count amount of product to be added
     */
    void addPurchase(const std::shared_ptr<Product>& p, uint32_t count);


    /**
     * @brief Generates a string containing invoice details.
     * @return info as string
     */
    [[nodiscard]] std::string buildInvoiceInfo() const;


    // ╔════════════════════════════════════════╗
    // ║           Getters & Setters            ║
    // ╚════════════════════════════════════════╝

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


    [[nodiscard]] std::vector<std::tuple<UUIDGen::UUID, uint32_t, ProductType, std::string, std::uint64_t> >
    getPurchaseList() const {
      return purchaseList;
    }
};


#endif //INVOICE_H

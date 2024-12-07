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
  /**
   * @brief Chronological history of product purchases.
   *
   * This vector stores tuples of product stock keeping units (SKU),
   * the corresponding quantity purchased, and the discount applied for each purchase.
   *
   * Each tuple represents a single purchase, where:
   * - SKU (std::uint32_t) : The unique product identifier.
   * - Quantity (std::uint16_t) : The amount of product purchased.
   * - Discount (double) : The discount applied to the purchase,
   *   expressed as a decimal (e.g., 0.10 for a 10% discount).
   *
   * Example: ⟨(SKU_1, qty_1, dsc_1), ..., (SKU_n, qty_n, dsc_n)⟩
   */
  std::vector<std::tuple<std::uint32_t, std::uint16_t, double> > purchaseHistory;

  std::string firstName;
  std::string lastName;
  std::string address;
  bool businessCustomer = false;

  public:
    // ╔════════════════════════════════════════╗
    // ║             Constructors               ║
    // ╚════════════════════════════════════════╝

    Customer(const std::vector<std::tuple<std::uint32_t, std::uint16_t, double> >& purchase_history,
             std::string first_name,
             std::string last_name,
             std::string address)
      : purchaseHistory(purchase_history),
        firstName(std::move(first_name)),
        lastName(std::move(last_name)),
        address(std::move(address)) {
    }

    // ╔════════════════════════════════════════╗
    // ║           Getters & Setters            ║
    // ╚════════════════════════════════════════╝

    // purchaseHistory
    /**
      * @brief Direct modification of this vector is discouraged.
      * @note Use addPurchase() or removePurchase() instead.
      * @note getPurchaseHistory() returns a mutable object.
      */
    [[nodiscard]] std::vector<std::tuple<std::uint32_t, std::uint16_t, double> >& getPurchaseHistory() {
      return purchaseHistory;
    }
    /**
      * @brief Direct modification of this vector is discouraged.
      * @note Use addPurchase() or removePurchase() instead.
      */
    void setPurchaseHistory(std::vector<std::tuple<std::uint32_t, std::uint16_t, double> > purchase_history) {
      purchaseHistory = std::move(purchase_history);
    }
    void addPurchase(std::tuple<std::uint32_t, std::uint16_t, double> purchase) {
      purchaseHistory.push_back(std::move(purchase));
    }
    /**
      * @brief Removes a purchase from the history by index.
      *
      * Removal of elements is discouraged, as it causes all subsequent elements
      * to shift, which can affect the validity of indices to nullify.
      *
      * @note The indices of all purchases after the removed one will be updated,
      *       which will break logic that depends on that order.
      */
    void removePurchase(const std::size_t index) {
      if (index < purchaseHistory.size()) {
        purchaseHistory.erase(purchaseHistory.begin() + static_cast<std::ptrdiff_t>(index));
      }
    }

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

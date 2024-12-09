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
#include "../../object/product/Product.h"
#include "../../object/product/tire/Tire.h"
#include "../../object/product/rim/Rim.h"

class Invoice {
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
  std::uint64_t discountRate = 0.0;

  public:
    explicit Invoice(const std::shared_ptr<Customer>& c)
      : customerId(c->getUUID()),
        firstName(c->getFirstName()),
        lastName(c->getLastName()),
        address(c->getAddress()),
        business(c->isBusinessCustomer()) {
    }

  void addPurchase(const std::shared_ptr<Product>& p, uint32_t count);

  void calculatePrice();
};


#endif //INVOICE_H

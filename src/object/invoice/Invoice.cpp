// ╔══════════════════════════════════════════════════════════════════════════════════╗
// ║ Name         : Invoice.cpp                                                      ║
// ║ Description  : Lorem ipsum dolor sit amet                                        ║
// ║                Lorem ipsum dolor sit amet                                        ║
// ║ Author(s)    : "Gilles Van pellicom" <r0997008@student.thomasmore.be>            ║
// ║ Date         : 2024/12/09                                                        ║                
// ║ Version      : 1.0                                                               ║
// ║ License      : GPL-3.0                                                           ║
// ╚══════════════════════════════════════════════════════════════════════════════════╝

#include "Invoice.h"

void Invoice::addPurchase(const std::shared_ptr<Product>& p, uint32_t count) {
  purchaseList.emplace_back(p->getUUID(),
                            count,
                            p->getType(),
                            p->getName(),
                            business ? p->getPriceBusiness() : p->getPriceIndividual());
}

void Invoice::calculatePrice() {
  noDiscountPrice = 0;
  discountRate = 0.0;

  unsigned int tireQty = 0;
  unsigned int rimQty = 0;

  // For all purchases
  for (const auto& [id, qty, type, name, price] : purchaseList) {
    noDiscountPrice += price;

    if (type == TIRE) {
      tireQty += qty;
    }
    if (type == RIM) {
      rimQty += qty;
    }
  }

  if (business) {
    // Discount starting from 10pcs
    if (const unsigned int totalQty = tireQty + rimQty;
      totalQty >= 10) {
      discountRate += 20;
    }
  } else {
    // Discount starting from 4 tires
    if (tireQty >= 4) {
      discountRate += 5;

      // Discount starting from 4 rims with 4 tires
      if (rimQty >= 4) {
        discountRate += 5;
      }
    }
  }

  // Apply discounts
  const std::uint64_t discount = noDiscountPrice * discountRate / 100;
  finalPrice = noDiscountPrice - discount;
}

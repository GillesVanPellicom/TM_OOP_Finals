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
  calculatePrice();
}


void Invoice::calculatePrice() {
  noDiscountPrice = 0;
  discountRate = 0.0;

  unsigned int tireQty = 0;
  unsigned int rimQty = 0;

  // For all purchases
  for (const auto& [id, qty, type, name, price] : purchaseList) {
    noDiscountPrice += qty * price; // Fix: Include quantity in the subtotal

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


std::string Invoice::buildInvoiceInfo() const {
  std::ostringstream info;

  // Invoice Header


  // Customer Information
  info << "Customer Name : " << firstName << " " << lastName << "\n"
      << "Address       : " << address << "\n"
      << "Customer Type : " << (business ? "Business" : "Individual") << "\n"
      << "Invoice Name  : " << invoiceName << "\n\n";

  // Purchase List Header
  info
      << "╔═════════════════════════════════════╦══════════╦══════════════╦══════════════╗\n"
      << "║ Product Name                        ║ Qty.     ║ Price        ║ Subtotal     ║\n"
      << "╠═════════════════════════════════════╬══════════╬══════════════╬══════════════╣\n";

  // Loop through the purchase list and add each product
  for (const auto& purchase : purchaseList) {
    auto [productID, quantity, productType, name, price] = purchase;
    std::uint64_t totalPrice = price * quantity;

    info
        << "║ " << std::setw(35) << std::left << name << " ║ "
        << std::setw(8) << std::right << quantity << " ║ "
        << std::setw(14) << std::right << Product::convertCentsToReadable(price) << " ║ "
        << std::setw(14) << std::right << Product::convertCentsToReadable(totalPrice) << " ║\n";
  }

  info << "╚═════════════════════════════════════╬══════════╩══════════════╬══════════════╣\n";

  // Total Price Information
  info << "                                      ║ Pre-Discount Price      ║ "
      << std::setw(14) << std::right << Product::convertCentsToReadable(noDiscountPrice) << " ║\n"
      << "                                      ║ Discount Rate           ║ "
      << std::setw(11) << std::right << discountRate << "%" << " ║\n"
      << "                                      ╠═════════════════════════╬══════════════╣\n"
      << "                                      ║ Final Price             ║ "
      << std::setw(14) << std::right << Product::convertCentsToReadable(finalPrice) << " ║\n"
      << "                                      ╚═════════════════════════╩══════════════╝\n\n";


  return info.str();
}

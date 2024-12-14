// ╔══════════════════════════════════════════════════════════════════════════════════╗
// ║ Name         : Invoice.cpp                                                       ║
// ║ Description  : Implementation of class Invoice                                   ║
// ║ Author(s)    : "Gilles Van pellicom" <r0997008@student.thomasmore.be>            ║
// ║ Date         : 2024/12/09                                                        ║
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
      discountRate += businessDiscount;
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
    const std::uint64_t totalPrice = price * quantity;

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


nlohmann::json Invoice::serialize() const {
  nlohmann::json j = {
    {"invoiceName", invoiceName},
    {"customerId", customerId},
    {"firstName", firstName},
    {"lastName", lastName},
    {"address", address},
    {"business", business},
    {"purchaseList", nlohmann::json::array()},
    {"noDiscountPrice", noDiscountPrice},
    {"finalPrice", finalPrice},
    {"discountRate", discountRate}
  };

  // Serialize the purchase list
  for (const auto& purchase : purchaseList) {
    j["purchaseList"].push_back({
        {"productID", std::get<0>(purchase)},
        {"qty", std::get<1>(purchase)},
        {"productType", static_cast<int>(std::get<2>(purchase))},
        {"name", std::get<3>(purchase)},
        {"price", std::get<4>(purchase)}
    });
  }

  return j;
}


void Invoice::deserialize(const nlohmann::json& j) {
  invoiceName = j.at("invoiceName").get<std::string>();
  customerId = j.at("customerId").get<std::string>();
  firstName = j.at("firstName").get<std::string>();
  lastName = j.at("lastName").get<std::string>();
  address = j.at("address").get<std::string>();
  business = j.at("business").get<bool>();
  noDiscountPrice = j.at("noDiscountPrice").get<std::uint64_t>();
  finalPrice = j.at("finalPrice").get<std::uint64_t>();
  discountRate = j.at("discountRate").get<std::uint64_t>();

  // Deserialize the purchase list
  purchaseList.clear();
  for (const auto& purchase : j.at("purchaseList")) {
    purchaseList.emplace_back(
        purchase.at("productID").get<std::string>(),
        purchase.at("qty").get<uint32_t>(),
        static_cast<ProductType>(purchase.at("productType").get<int>()),
        purchase.at("name").get<std::string>(),
        purchase.at("price").get<std::uint64_t>()
    );
  }
}

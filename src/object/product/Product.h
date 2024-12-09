// ╔══════════════════════════════════════════════════════════════════════════════════╗
// ║ Name         : Product.h (abstract)                                                      ║
// ║ Description  : Lorem ipsum dolor sit amet                                        ║
// ║                Lorem ipsum dolor sit amet                                        ║
// ║ Author(s)    : "Gilles Van pellicom" <r0997008@student.thomasmore.be>            ║
// ║ Date         : 2024/12/07                                                        ║
// ╚══════════════════════════════════════════════════════════════════════════════════╝

#ifndef PRODUCT_H
#define PRODUCT_H


#include <string>
#include <json.hpp>
#include <utility>

#include "../../util/IdGen.hpp"

enum ProductType {
  TIRE,
  RIM
};

class Product {
  // ╔════════════════════════════════════════╗
  // ║               Variables                ║
  // ╚════════════════════════════════════════╝

  std::string name [[maybe_unused]];
  std::string manufacturer [[maybe_unused]];
  std::uint32_t diameter{}; // in mm
  ProductType type{};

  std::uint32_t stockCount{}; // in ℕ_{0}
  std::uint64_t priceIndividual{}; // in cents, euro
  std::uint64_t priceBusiness{}; // in cents, euro

  UUIDGen::UUID uuid = UUIDGen::generateUUID();

  public:
    // ╔════════════════════════════════════════╗
    // ║             Constructors               ║
    // ╚════════════════════════════════════════╝

    Product(std::string name,
            std::string manufacturer,
            const std::uint32_t diameter,
            const ProductType type,
            const std::uint32_t stock_count,
            const std::uint64_t price_individual,
            const std::uint64_t price_business)
      : name(std::move(name)),
        manufacturer(std::move(manufacturer)),
        diameter(diameter),
        type(type),
        stockCount(stock_count),
        priceIndividual(price_individual),
        priceBusiness(price_business) {
    }

    Product() = default;

    static std::string convertCentsToReadable(const uint64_t cents) {
      const uint64_t euro = cents / 100;
      const uint64_t remainderCents = cents % 100;

      return "€" + std::to_string(euro) + "," + std::to_string(remainderCents);
    }

    // ╔════════════════════════════════════════╗
    // ║           Getters & Setters            ║
    // ╚════════════════════════════════════════╝

    // name
    // function can't guarantee const because of the reference so that's why that also has to be const
    [[nodiscard]] const std::string& getName() const {
      return name;
    }
    void setName(std::string name) {
      this->name = std::move(name);
    }

    // manufacturer
    [[nodiscard]] const std::string& getManufacturer() const {
      return manufacturer;
    }
    void setManufacturer(std::string manufacturer) {
      this->manufacturer = std::move(manufacturer);
    }

    // diameter
    [[nodiscard]] std::uint32_t getDiameter() const {
      return diameter;
    }
    void setDiameter(const std::uint32_t diameter) {
      this->diameter = diameter;
    }

    // stockCount
    [[nodiscard]] std::uint32_t getStockCount() const {
      return stockCount;
    }
    void setStockCount(const std::uint32_t stock_count) {
      stockCount = stock_count;
    }
    /**
     * @brief Decrements stock by given amount
     * @param stock_count_decrement
     * @return true if success, else false
     */
    bool decrementStockCount(const std::uint32_t stock_count_decrement) {
      if (stockCount - stock_count_decrement < 0) return false;
      stockCount -= stock_count_decrement;
      return true;
    }

    // priceIndividual
    [[nodiscard]] std::uint64_t getPriceIndividual() const {
      return priceIndividual;
    }
    void setPriceIndividual(const std::uint64_t price_individual) {
      priceIndividual = price_individual;
    }

    // priceBusiness
    [[nodiscard]] std::uint64_t getPriceBusiness() const {
      return priceBusiness;
    }
    void setPriceBusiness(const std::uint64_t price_business) {
      priceBusiness = price_business;
    }

    // type
    [[nodiscard]] bool instanceOf(ProductType type) const {
      return this->type == type;
    }
    [[nodiscard]] ProductType getType() const {
      return type;
    }
    [[nodiscard]] std::string getTypeAsString() const {
      switch (this->type) {
        case TIRE: { return "tire"; }
        case RIM: { return "rim"; }
      }
      return "error";
    }
    void setTypeAsString(const std::string& typeStr) {
      if (typeStr == "tire") {
        this->type = TIRE;
      } else if (typeStr == "rim") {
        this->type = RIM;
      } else {
        this->type = static_cast<ProductType>(-1); // Invalid type
      }
    }

    // UUID
    [[nodiscard]] UUIDGen::UUID getUUID() const {
      return uuid;
    }
    void setUUID(const UUIDGen::UUID& uuid) {
      this->uuid = uuid;
    }

    virtual ~Product() = default;

    // ╔════════════════════════════════════════╗
    // ║               functions                ║
    // ╚════════════════════════════════════════╝

    /**
     * @brief Serializes this specific object, used in recursive serialization.
     * @return Object as JSON
     */
    [[nodiscard]] virtual nlohmann::json serialize() = 0;

    /**
   * @brief Deserializes this specific object, used in recursive deserialization.
   */
    virtual void deserialize(const nlohmann::json& j) = 0;
};

#endif //PRODUCT_H

// ╔══════════════════════════════════════════════════════════════════════════════════╗
// ║ Name         : Product.h                                                         ║
// ║ Description  : Definition of the abstract class Product                          ║
// ║ Child(ren)   : Tire.h, Rim.h                                                     ║
// ║ Author(s)    : "Gilles Van pellicom" <r0997008@student.thomasmore.be>            ║
// ║ Date         : 2024/12/07                                                        ║
// ╚══════════════════════════════════════════════════════════════════════════════════╝

#ifndef PRODUCT_H
#define PRODUCT_H

// STD
#include <string>
#include <json.hpp>
#include <utility>

// Local
#include "../../util/IdGen.hpp"

/**
 * @brief Enum used for type safety for product type
 */
enum ProductType {
  TIRE,
  RIM
};


/**
 * @brief Abstract class representing a product.
 *
 * This class defines common attributes and behaviors for a product, such as name,
 * manufacturer, price, and stock count. It provides methods for serializing and
 * deserializing the product to/from JSON, and for handling stock management.
 *
 * @note Product can be either a Tire or Rim, as denoted by the ProductType enum.
 *
 * @see ProductType, UUIDGen
 */
class Product {
  private:
    // ╔════════════════════════════════════════╗
    // ║              Attributes                ║
    // ╚════════════════════════════════════════╝

    std::string name;
    std::string manufacturer;
    std::uint32_t diameter; // in mm
    ProductType type;

    std::uint32_t stockCount; // in ℕ_{0}^{+}
    std::uint64_t priceIndividual; // in cents, euro
    std::uint64_t priceBusiness; // in cents, euro

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


    // ╔════════════════════════════════════════╗
    // ║            Public Methods              ║
    // ╚════════════════════════════════════════╝

    static std::string convertCentsToReadable(const uint64_t cents) {
      const uint64_t euro = cents / 100;
      const uint64_t remainderCents = cents % 100;

      return "€" + std::to_string(euro) + "," + std::to_string(remainderCents);
    }


    /**
     * @brief Serializes class to JSON.
     * Used for recursive serialization
     * @return class serialized as JSON object
     */
    [[nodiscard]] virtual nlohmann::json serialize() = 0;


    /**
     * @brief Deserializes class from JSON.
     * Used for recursive deserialization
     * @param j JSON object from which to de-serialize
     */
    virtual void deserialize(const nlohmann::json& j) = 0;


    /**
     * @brief Generates a string containing product details.
     * @return info as string
     */
    virtual std::string buildProductInfo() = 0;


    /**
     * @brief Checks if enough stock is left.
     * Taking into account the fact that stock is unsigned
     * @param stock_count_decrement amount to decrement stock with
     * @return true if possible, false if not
     */
    [[nodiscard]] bool isDecrementPossible(const std::uint32_t stock_count_decrement) const {
      return stock_count_decrement <= stockCount;
    }


    // ╔════════════════════════════════════════╗
    // ║           Getters & Setters            ║
    // ╚════════════════════════════════════════╝

    [[nodiscard]] const std::string& getName() const {
      return name;
    }
    void setName(std::string name) {
      this->name = std::move(name);
    }


    [[nodiscard]] const std::string& getManufacturer() const {
      return manufacturer;
    }
    void setManufacturer(std::string manufacturer) {
      this->manufacturer = std::move(manufacturer);
    }


    [[nodiscard]] std::uint32_t getDiameter() const {
      return diameter;
    }
    void setDiameter(const std::uint32_t diameter) {
      this->diameter = diameter;
    }


    [[nodiscard]] std::uint32_t getStockCount() const {
      return stockCount;
    }
    void setStockCount(const std::uint32_t stock_count) {
      stockCount = stock_count;
    }

    /**
     * @brief Decrements stock by given amount.
     * @param stock_count_decrement
     * @return true if success, else false
     */
    bool decrementStockCount(const std::uint32_t stock_count_decrement) {
      if (stock_count_decrement > stockCount) return false;
      stockCount -= stock_count_decrement;
      return true;
    }


    [[nodiscard]] std::uint64_t getPriceIndividual() const {
      return priceIndividual;
    }
    void setPriceIndividual(const std::uint64_t price_individual) {
      priceIndividual = price_individual;
    }


    [[nodiscard]] std::uint64_t getPriceBusiness() const {
      return priceBusiness;
    }
    void setPriceBusiness(const std::uint64_t price_business) {
      priceBusiness = price_business;
    }


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

    // ╔════════════════════════════════════════╗
    // ║              Destructors               ║
    // ╚════════════════════════════════════════╝


    virtual ~Product() = default;
};

#endif //PRODUCT_H

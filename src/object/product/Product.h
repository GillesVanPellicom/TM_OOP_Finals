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

class Product {
  // ╔════════════════════════════════════════╗
  // ║               Variables                ║
  // ╚════════════════════════════════════════╝

  std::string name [[maybe_unused]];
  std::string manufacturer [[maybe_unused]];
  std::uint32_t diameter [[maybe_unused]]; // in mm

  std::uint32_t stockCount [[maybe_unused]]; // in ℕ_{0}
  std::uint64_t priceIndividual [[maybe_unused]]; // in cents, euro
  std::uint64_t priceBusiness [[maybe_unused]]; // in cents, euro

  public:
    // ╔════════════════════════════════════════╗
    // ║             Constructors               ║
    // ╚════════════════════════════════════════╝

    Product(std::string name,
            std::string manufacturer,
            const std::uint32_t diameter,
            const std::uint32_t stock_count,
            const std::uint64_t price_individual,
            const std::uint64_t price_business)
      : name(std::move(name)),
        manufacturer(std::move(manufacturer)),
        diameter(diameter),
        stockCount(stock_count),
        priceIndividual(price_individual),
        priceBusiness(price_business) {
    }

    // ╔════════════════════════════════════════╗
    // ║           Getters & Setters            ║
    // ╚════════════════════════════════════════╝

    // Name
    // function can't guarantee const because of the reference so that's why that also has to be const
    [[nodiscard]] const std::string& getName() const {
      return name;
    }
    void setName(std::string name) {
      this->name = std::move(name);
    }

    // Manufacturer
    [[nodiscard]] const std::string& getManufacturer() const {
      return manufacturer;
    }
    void setManufacturer(std::string manufacturer) {
      this->manufacturer = std::move(manufacturer);
    }

    // Diameter
    [[nodiscard]] std::uint32_t getDiameter() const {
      return diameter;
    }
    void setDiameter(const std::uint32_t diameter) {
      this->diameter = diameter;
    }

    // StockCount
    [[nodiscard]] std::uint32_t getStockCount() const {
      return stockCount;
    }
    void setStockCount(const std::uint32_t stock_count) {
      stockCount = stock_count;
    }
    void decrementStockCount(const std::uint32_t stock_count_decrement) {
      stockCount -= stock_count_decrement;
    }

    // Virtual Destructor for Polymorphism
    virtual ~Product() = default;
};

#endif //PRODUCT_H

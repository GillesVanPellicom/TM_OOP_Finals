// ╔══════════════════════════════════════════════════════════════════════════════════╗
// ║ Name         : Tire.h                                                            ║
// ║ Description  : Definition of the class Tire                                      ║
// ║ Inherit(s)   : Product.h                                                         ║
// ║ Author(s)    : "Gilles Van pellicom" <r0997008@student.thomasmore.be>            ║
// ║ Date         : 2024/12/07                                                        ║
// ╚══════════════════════════════════════════════════════════════════════════════════╝

#ifndef TIRE_H
#define TIRE_H


#include "../Product.h"


/**
 * @brief Concrete class representing a tire product.
 *
 * The `Tire` class extends the abstract `Product` class and provides specific
 * functionality for handling tire-related attributes, including width, height, and
 * speed index. It also implements methods for serializing, deserializing, and
 * generating detailed information about the tire product.
 *
 * @note This class is a final class and cannot be subclassed.
 *
 * @see Product
 */
class Tire final : public Product {
  private:
    // ╔════════════════════════════════════════╗
    // ║              Attributes                ║
    // ╚════════════════════════════════════════╝

    std::uint32_t width; // in mm
    std::uint32_t height; // in mm
    char speedIndex;

  public:
    // ╔════════════════════════════════════════╗
    // ║             Constructors               ║
    // ╚════════════════════════════════════════╝

    Tire(std::string name,
         std::string manufacturer,
         const std::uint32_t diameter,
         const std::uint32_t stock_count,
         const std::uint64_t price_individual,
         const std::uint64_t price_business,
         const std::uint32_t width,
         const std::uint32_t height,
         const char speed_index)
      : Product(std::move(name),
                std::move(manufacturer),
                diameter,
                TIRE,
                stock_count,
                price_individual,
                price_business),
        width(width),
        height(height),
        speedIndex(speed_index) {
    }


    explicit Tire(const nlohmann::json& j): Product() {
      Tire::deserialize(j);
    }

    // ╔════════════════════════════════════════╗
    // ║           Getters & Setters            ║
    // ╚════════════════════════════════════════╝

    // Width
    [[nodiscard]] std::uint32_t getWidth() const {
      return width;
    }
    void setWidth(const std::uint32_t width) {
      this->width = width;
    }

    // Height
    [[nodiscard]] std::uint32_t getHeight() const {
      return height;
    }
    void setHeight(const std::uint32_t height) {
      this->height = height;
    }

    // SpeedIndex
    [[nodiscard]] char getSpeedIndex() const {
      return speedIndex;
    }
    void setSpeedIndex(const char speed_index) {
      this->speedIndex = speed_index;
    }


    // ╔════════════════════════════════════════╗
    // ║              Destructors               ║
    // ╚════════════════════════════════════════╝

    ~Tire() override = default;


    // ╔════════════════════════════════════════╗
    // ║            Public Methods              ║
    // ╚════════════════════════════════════════╝

    /**
      * @brief Serializes this specific object, used in recursive serialization.
      * @return Object as JSON
      */
    [[nodiscard]] nlohmann::json serialize() override;

    /**
      * @brief Deserializes this specific object, used in recursive deserialization.
      */
    void deserialize(const nlohmann::json& j) override;


    /**
      * @brief Generates a string containing product details.
      * @return info as string
      */
    std::string buildProductInfo() override;
};


#endif //TIRE_H

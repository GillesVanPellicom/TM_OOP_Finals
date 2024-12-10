// ╔══════════════════════════════════════════════════════════════════════════════════╗
// ║ Name         : Rim.h                                                             ║
// ║ Description  : Definition of the class Rim                                       ║
// ║ Inherit(s)   : Product.h                                                         ║
// ║ Author(s)    : "Gilles Van pellicom" <r0997008@student.thomasmore.be>            ║
// ║ Date         : 2024/12/07                                                        ║
// ╚══════════════════════════════════════════════════════════════════════════════════╝

#ifndef RIM_H
#define RIM_H

// STD
#include <utility>

// Local
#include "../Product.h"

/**
  * @brief Enum used for type safety for material type
  */
enum RimMaterial {
  STEEL,
  ALUMINIUM
};


/**
 * @brief Concrete class representing a rim product.
 *
 * The `Rim` class extends the abstract `Product` class and provides specific
 * functionality for handling rim-related attributes, including width, color, and
 * material type. It also implements methods for serializing, deserializing, and
 * generating detailed information about the rim product.
 *
 * @note This class is a final class and cannot be subclassed.
 *
 * @see Product
 */
class Rim final : public Product {
  float width; // in inch
  std::string color;
  RimMaterial material; // enum

  public:
    // ╔════════════════════════════════════════╗
    // ║             Constructors               ║
    // ╚════════════════════════════════════════╝

    Rim(std::string name,
        std::string manufacturer,
        const std::uint32_t diameter,
        const std::uint32_t stock_count,
        const std::uint64_t price_individual,
        const std::uint64_t price_business,
        const float width,
        std::string color,
        RimMaterial material)
      : Product(std::move(name),
                std::move(manufacturer),
                diameter,
                RIM,
                stock_count,
                price_individual,
                price_business),
        width(width),
        color(std::move(color)),
        material(material) {
    }


    explicit Rim(const nlohmann::json& j): Product() {
      Rim::deserialize(j);
    }


    // ╔════════════════════════════════════════╗
    // ║           Getters & Setters            ║
    // ╚════════════════════════════════════════╝

    [[nodiscard]] float getWidth() const {
      return width;
    }
    void setWidth(const float width) {
      this->width = width;
    }


    [[nodiscard]] const std::string& getColor() const {
      return color;
    }
    void setColor(std::string color) {
      this->color = std::move(color);
    }


    [[nodiscard]] RimMaterial getMaterial() const {
      return material;
    }
    [[nodiscard]] std::string getMaterialAsString() const {
      switch (material) {
        case STEEL: { return "steel"; }
        case ALUMINIUM: { return "aluminium"; }
      }
      return "error";
    }
    void setMaterialAsString(const std::string& materialStr) {
      if (materialStr == "steel") {
        this->material = STEEL;
      } else if (materialStr == "aluminium") {
        this->material = ALUMINIUM;
      } else {
        this->material = static_cast<RimMaterial>(-1); // Invalid material
      }
    }
    void setMaterial(const RimMaterial material) {
      this->material = material;
    }


    // ╔════════════════════════════════════════╗
    // ║              Destructors               ║
    // ╚════════════════════════════════════════╝

    ~Rim() override = default;


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


#endif //RIM_H

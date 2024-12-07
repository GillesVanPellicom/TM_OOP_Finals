// ╔══════════════════════════════════════════════════════════════════════════════════╗
// ║ Name         : Rim.h                                                      ║
// ║ Description  : Lorem ipsum dolor sit amet                                        ║
// ║                Lorem ipsum dolor sit amet                                        ║
// ║ Author(s)    : "Gilles Van pellicom" <r0997008@student.thomasmore.be>            ║
// ║ Date         : 2024/12/07                                                        ║                
// ║ Version      : 1.0                                                               ║
// ║ License      : GPL-3.0                                                           ║
// ╚══════════════════════════════════════════════════════════════════════════════════╝

#ifndef RIM_H
#define RIM_H

#include <utility>

#include "../Product.h"

enum RimMaterial {
  STEEL,
  ALUMINIUM
};

class Rim : public Product {
  float width [[maybe_unused]]; // in inch
  std::string color [[maybe_unused]];
  RimMaterial material [[maybe_unused]]; // enum

  public:
    // ╔════════════════════════════════════════╗
    // ║             Constructors               ║
    // ╚════════════════════════════════════════╝

    Rim(std::string name,
        std::string manufacturer,
        const std::uint32_t diameter,
        const unsigned int stock_count,
        const std::uint64_t price_individual,
        const std::uint64_t price_business,
        const float width,
        std::string color,
        RimMaterial material)
      : Product(std::move(name), std::move(manufacturer), diameter, stock_count, price_individual, price_business),
        width(width),
        color(std::move(color)),
        material(material) {
    }


    // ╔════════════════════════════════════════╗
    // ║           Getters & Setters            ║
    // ╚════════════════════════════════════════╝

    // Width
    [[nodiscard]] float getWidth() const {
      return width;
    }
    void setWidth(const float width) {
      this->width = width;
    }

    // Color
    [[nodiscard]] const std::string& getColor() const {
      return color;
    }
    void setColor(std::string color) {
      this->color = std::move(color);
    }

    // Material
    [[nodiscard]] RimMaterial getMaterial() const {
      return material;
    }
    void setMaterial(const RimMaterial material) {
      this->material = material;
    }
};


#endif //RIM_H

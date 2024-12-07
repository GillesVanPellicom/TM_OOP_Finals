// ╔══════════════════════════════════════════════════════════════════════════════════╗
// ║ Name         : Tire.h                                                      ║
// ║ Description  : Lorem ipsum dolor sit amet                                        ║
// ║                Lorem ipsum dolor sit amet                                        ║
// ║ Author(s)    : "Gilles Van pellicom" <r0997008@student.thomasmore.be>            ║
// ║ Date         : 2024/12/07                                                        ║                
// ║ Version      : 1.0                                                               ║
// ║ License      : GPL-3.0                                                           ║
// ╚══════════════════════════════════════════════════════════════════════════════════╝

#ifndef TIRE_H
#define TIRE_H

#include "../Product.h"


class Tire : public Product {
  std::uint32_t width [[maybe_unused]]; // in mm
  std::uint32_t height [[maybe_unused]]; // in mm
  char speedIndex [[maybe_unused]];

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
};


#endif //TIRE_H

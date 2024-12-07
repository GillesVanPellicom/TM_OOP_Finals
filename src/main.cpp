// STL
#include <iostream>

// Local
#include "object/product/tire/Tire.h"
#include "object/product/rim/Rim.h"

int main() {
  // Vector to hold shared_ptr to Product objects
  std::vector<std::shared_ptr<Product>> products;

  // Loop to create a few Tire objects and store them in the vector
  products.reserve(5);
for (int i = 0; i < 5; ++i) {
    // Creating a new Tire object and adding it to the vector
    products.push_back(std::make_shared<Tire>(
        "Tire " + std::to_string(i + 1),  // name
        "Manufacturer " + std::to_string(i + 1),  // manufacturer
        17 + i,  // diameter
        100 + i * 10,  // stock count
        5000 + i * 100,  // price individual
        4500 + i * 90,  // price business
        200 + i * 10,  // width
        55 + i,  // height
        'H' + (i % 5)  // speed index (cycling through 'H', 'V', etc.)
    ));
  }

  // Output the tire names as an example of access
  for (const auto& product : products) {
    if (const auto tire = std::dynamic_pointer_cast<Tire>(product)) {
      std::cout << tire->getName() << std::endl;
    }
  }

  return 0;
}
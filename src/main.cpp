// STL
#include <iostream>

// Local
#include "object/product/tire/Tire.h"
#include "object/product/rim/Rim.h"
#include "menu/ChoiceMenu.hpp"
#include "menu/SequentialMenu.hpp"

void testFn1() {
  std::cout << "test 1" << std::endl;
}

void testFn2() {
  std::cout << "test 2" << std::endl;
}

void setupSession() {
}

void initMenu() {
}

int main() {
  //   // Vector to hold shared_ptr to Product objects
  //   std::vector<std::shared_ptr<Product>> products;
  //
  //   // Loop to create a few Tire objects and store them in the vector
  //   products.reserve(5);
  // for (int i = 0; i < 5; ++i) {
  //     // Creating a new Tire object and adding it to the vector
  //     products.push_back(std::make_shared<Tire>(
  //         "Tire " + std::to_string(i + 1),  // name
  //         "Manufacturer " + std::to_string(i + 1),  // manufacturer
  //         17 + i,  // diameter
  //         100 + i * 10,  // stock count
  //         5000 + i * 100,  // price individual
  //         4500 + i * 90,  // price business
  //         200 + i * 10,  // width
  //         55 + i,  // height
  //         'H' + (i % 5)  // speed index (cycling through 'H', 'V', etc.)
  //     ));
  //   }
  //
  //   // Output the tire names as an example of access
  //   for (const auto& product : products) {
  //     if (const auto tire = std::dynamic_pointer_cast<Tire>(product)) {
  //       std::cout << tire->getName() << std::endl;
  //     }
  //   }


  ChoiceMenu subMenu("Change inventory");
  subMenu.addOption("Add item", testFn1);
  subMenu.addOption("Remove item", testFn2);


  SequentialMenu seqMenu("Sequential Input");
  seqMenu.addCollection("Car name");
  seqMenu.addCollection("Amount");
  seqMenu.setHandler([](const std::vector<std::string>& inputs) {
    const std::string& carName = inputs[0];
    const int amount = std::stoi(inputs[1]);

    std::cout << "\nYou chose " << carName << " with amount " << amount << ".\n";
  });


  ChoiceMenu mainMenu("Main menu");
  mainMenu.addSubMenu("Sequential menu", seqMenu);
  mainMenu.addOption("Theing", []() { std::cout << "You chose this theing.\n"; });
  mainMenu.addSubMenu("Manage items", subMenu);

  // Display the main menu
  mainMenu.display();

  return 0;
}

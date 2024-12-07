// STL
#include <iostream>

// Local
#include "object/product/tire/Tire.h"
#include "object/product/rim/Rim.h"
#include "menu/ChoiceMenu.hpp"
#include "menu/SequentialMenu.hpp"
#include "object/customer/Customer.h"
#include "object/user/User.h"

std::unordered_map<std::string, std::shared_ptr<User> > users;
std::vector<std::shared_ptr<Product> > products;
std::string cuid;


void setupSession() {
  while (true) {
    // get input
    std::string uid;
    std::cout << "\nPlease enter your UID:\033[0m\n> ";
    std::getline(std::cin, uid);
    std::cout << "\033[0m\n" << std::endl;

    // Check "password"
    if (!users.contains(uid)) {
      std::cout << "\033[1;31mInvalid UID. Please try again.\033[0m\n";
      continue;
    }

    // Set session variables
    cuid = uid;

    // Be nice to the user
    std::cout << "Welcome " << users[cuid]->getUserName() << "\n" << std::endl;
    break;
  }
}

void initMenu() {
  auto fullStockMenu = std::make_shared<ChoiceMenu>("Stock menu");
  for (const auto& product : products) {
    fullStockMenu->addOption(
      product->getName(),
      [fullStockMenu, product]() {
        // FIXME: addStockMenu exits to inspectMenu whilst parent is fullStockMenu ¯\_(ツ)_/¯
        const auto addStockMenu = std::make_shared<SequentialMenu>("Stock menu");
        addStockMenu->addCollection("Amount of stock to add");
        addStockMenu->setHandler([product](const std::vector<std::string>& inputs) {
          product->setStockCount(product->getStockCount() + std::stoi(inputs[0]));
        });

        std::string productInfo =
            "Name: " + product->getName() +
            "\nManufacturer: " + product->getManufacturer() +
            "\nType: " + product->getTypeAsString() +
            "\nDiameter: " + std::to_string(product->getDiameter()) + " inch";


        if (product->instanceOf(TIRE)) {
          // if tire, add tire specific data
          const auto tire = std::dynamic_pointer_cast<Tire>(product);
          productInfo +=
              "\nWidth: " + std::to_string(tire->getWidth()) + " mm"
              "\nHeight: " + std::to_string(tire->getHeight()) + " mm"
              "\nSpeed Index: " + std::string(1, tire->getSpeedIndex());
        } else if (product->instanceOf(RIM)) {
          // if rim, add rim specific data
          const auto rim = std::dynamic_pointer_cast<Rim>(product);
          productInfo +=
              "\nWidth: " + std::to_string(rim->getWidth()) + //FIXME: add mm, inch fzo
              "\nColor: " + rim->getColor() +
              "\nMaterial: " + rim->getMaterialAsString();
        }

        productInfo +=
            "\nStock Count: " + std::to_string(product->getStockCount()) +
            "\nPrice (Individual): " + Product::convertCentsToReadable(
              product->getPriceIndividual()) +
            "\nPrice (Business) : " + Product::convertCentsToReadable(
              product->getPriceBusiness());

        const auto inspectMenu = std::make_shared<ChoiceMenu>("Inspect Product", fullStockMenu);
        inspectMenu->addOption("Add stock", addStockMenu);
        addStockMenu->setParentMenu(fullStockMenu);
        inspectMenu->setSuffixText(productInfo);
        inspectMenu->display();
      });
  }


  const auto stockMenu = std::make_shared<ChoiceMenu>("Stock menu");
  stockMenu->addOption("Show full stock", fullStockMenu);


  const auto mainMenu = std::make_shared<ChoiceMenu>("Main menu");
  mainMenu->addOption("View stock", stockMenu);

  // Display the main menu
  mainMenu->display();
}


void init() {
  // FIXME: dev stuff, remove when serialization works
  users["GYLS"] = std::make_shared<User>("Gilles", ADMIN);
  users["ALSTY"] = std::make_shared<User>("Alec", EMPLOYEE);

  products.emplace_back(std::make_shared<Tire>(
    "WinterGrip X200",
    "Goodyear",
    18,
    50,
    12075,
    11550,
    225,
    55,
    'H'
  ));

  products.emplace_back(std::make_shared<Tire>(
    "AllSeason Pro",
    "Michelin",
    16,
    100,
    9500,
    8800,
    205,
    60,
    'T'
  ));

  products.emplace_back(std::make_shared<Tire>(
    "SportMaxx Ultra",
    "Dunlop",
    19,
    30,
    14500,
    13500,
    245,
    40,
    'V'
  ));

  products.emplace_back(std::make_shared<Tire>(
    "EcoDrive E300",
    "Bridgestone",
    15,
    75,
    8000,
    7550,
    185,
    65,
    'S'
  ));

  products.emplace_back(std::make_shared<Tire>(
    "RoadMaster R500",
    "Continental",
    20,
    20,
    20000,
    19000,
    275,
    35,
    'Y'
  ));

  products.emplace_back(std::make_shared<Rim>(
    "AlloyStyle A500",
    "BBS",
    18,
    40,
    25000,
    24000,
    8.5f,
    "Silver",
    ALUMINIUM
  ));

  products.emplace_back(std::make_shared<Rim>(
    "SteelBasic S200",
    "OZ Racing",
    16,
    100,
    8000,
    7500,
    6.5f,
    "Black",
    STEEL
  ));

  products.emplace_back(std::make_shared<Rim>(
    "CarbonXtreme C800",
    "Enkei",
    19,
    10,
    55000,
    53000,
    9.0f,
    "Matte Black",
    ALUMINIUM
  ));

  products.emplace_back(std::make_shared<Rim>(
    "ChromeClassic R300",
    "American Racing",
    17,
    25,
    20000,
    19000,
    7.0f,
    "Chrome",
    ALUMINIUM
  ));

  products.emplace_back(std::make_shared<Rim>(
    "OffRoad Master OR900",
    "Method Race Wheels",
    20,
    15,
    35000,
    34000,
    10.0f,
    "Gunmetal",
    STEEL
  ));

  setupSession();
  initMenu();
}


int main() {
  init();
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


  // ChoiceMenu subMenu("Change inventory");
  // subMenu.addOption("Add item", testFn1);
  // subMenu.addOption("Remove item", testFn2);
  //
  //
  // SequentialMenu seqMenu("Sequential Input");
  // seqMenu.addCollection("Car name");
  // seqMenu.addCollection("Amount");
  // seqMenu.setHandler([](const std::vector<std::string>& inputs) {
  //   const std::string& carName = inputs[0];
  //   const int amount = std::stoi(inputs[1]);
  //
  //   std::cout << "\nYou chose " << carName << " with amount " << amount << ".\n";
  // });
  //
  //
  // ChoiceMenu mainMenu("Main menu");
  // mainMenu.addOption("Sequential menu", seqMenu);
  // mainMenu.addOption("Theing", []() { std::cout << "You chose this theing.\n"; });
  // mainMenu.addOption("Manage items", subMenu);

  // Display the main menu
  // mainMenu.display();

  return 0;
}

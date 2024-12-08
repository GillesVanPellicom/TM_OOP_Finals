// STL
#include <iostream>

// Local
#include <fstream>

#include "object/product/tire/Tire.h"
#include "object/product/rim/Rim.h"
#include "menu/ChoiceMenu.hpp"
#include "menu/SequentialMenu.hpp"
#include "object/customer/Customer.h"
#include "object/user/User.h"

std::unordered_map<std::string, std::shared_ptr<User> > users;
std::vector<std::shared_ptr<Product> > products;
std::string cuid;



/**
 * @brief Saves contents of program to memory
 *
 * @param filePath Path to json save file
 */
void serialize(const std::string& filePath) {
  nlohmann::json j; // JSON root


  // Products
  // Create a JSON array to hold all the products
  nlohmann::json products_array = nlohmann::json::array();
  for (const auto& p : products) {
    // Add to the products array
    products_array.push_back(p->serialize());
  }
  j["products"] = products_array;
  // END Products

  // Write the JSON to a file
  std::ofstream outputFile(filePath);
  if (!outputFile.is_open()) {
    std::cerr << "Could not open the file at " << filePath << std::endl;
    return;
  }

  outputFile << j.dump(4);
  outputFile.close();
  std::cout << "Save complete.\n" << std::endl;
}


/**
 * @brief Reads in contents of program from memory
 *
 * @param filePath Path to json save file
 */
void deserialize(const std::string& filePath) {
  // Read JSON from file
  std::ifstream inputFile(filePath);
  if (!inputFile.is_open()) {
    std::cerr << "Could not open the file at " << filePath << std::endl;
    return;
  }

  nlohmann::json j;
  inputFile >> j;

  if (j.contains("products") && j["products"].is_array()) {
    for (const auto& product_json : j["products"]) {
      std::string type = product_json.at("type").get<std::string>();

      if (type == "tire") {
        auto tire = std::make_shared<Tire>(product_json);
        products.emplace_back(tire);
      } else if (type == "rim") {
        auto rim = std::make_shared<Rim>(product_json);
        products.emplace_back(rim);
      }
    }
  }
  std::cout << "Loading from memory completed.\n" << std::endl;
}


/**
 * @brief Asks user for their UID and sets up session permissions accordingly
 */
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


/**
 * @brief Entrypoint for the menu tree. Loads main menu.
 */
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
  mainMenu->addOption("Save changes", [] { serialize("../mem.json.bak"); });

  // Display the main menu
  mainMenu->display();
}


/**
 * @brief Handles full software initialization.
 */
void init() {
  deserialize("../mem.json.bak");

  // FIXME: dev stuff, remove when serialization works
  users["GYLS"] = std::make_shared<User>("Gilles", ADMIN);
  users["ALSTY"] = std::make_shared<User>("Alec", EMPLOYEE);

  setupSession();
  initMenu();
}



int main() {
  init();
  return 0;
}

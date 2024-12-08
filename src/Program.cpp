// ╔══════════════════════════════════════════════════════════════════════════════════╗
// ║ Name         : Program.cpp                                                      ║
// ║ Description  : Lorem ipsum dolor sit amet                                        ║
// ║                Lorem ipsum dolor sit amet                                        ║
// ║ Author(s)    : "Gilles Van pellicom" <r0997008@student.thomasmore.be>            ║
// ║ Date         : 2024/12/08                                                        ║
// ╚══════════════════════════════════════════════════════════════════════════════════╝

#include "Program.h"


void Program::serialize(const std::string& filePath) const {
  nlohmann::json j; // JSON root

  // Products
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


void Program::deserialize(const std::string& filePath) {
  // Read JSON from file
  std::ifstream inputFile(filePath);
  if (!inputFile.is_open()) {
    std::cerr << "Could not open the file at " << filePath << std::endl;
    return;
  }
  nlohmann::json j;
  inputFile >> j;

  // Products
  if (j.contains("products") && j["products"].is_array()) {
    for (const auto& product_json : j["products"]) {
      if (auto type = product_json.at("type").get<std::string>();
        type == "tire") {
        const auto tire = std::make_shared<Tire>(product_json);
        products.emplace_back(tire);
      } else if (type == "rim") {
        const auto rim = std::make_shared<Rim>(product_json);
        products.emplace_back(rim);
      }
    }
  }
  // END Products

  std::cout << "Loading from memory completed.\n" << std::endl;
}


void Program::setupSession() {
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


void Program::initMenu() const {
  const auto stockMenu = std::make_shared<ChoiceMenu>("Stock menu");

  const auto fullStockMenu = createFullStockMenu();
  const auto filteredStockMenu = createFilterMenu();

  filteredStockMenu->setHandler([&filteredStockMenu, &stockMenu, this](const std::vector<std::string>& inputs) {
    const auto filteredFullStockMenu = createFullStockMenu(inputs[0]);
    filteredStockMenu->setParentMenu(filteredFullStockMenu);
    filteredFullStockMenu->setParentMenu(stockMenu);
  });


  stockMenu->addOption("Show full stock", fullStockMenu);
  stockMenu->addOption("Show filtered stock", filteredStockMenu);

  const auto mainMenu = std::make_shared<ChoiceMenu>("Main menu");
  mainMenu->addOption("View stock", stockMenu);
  mainMenu->addOption("Save changes", [this] { serialize("../mem.json.bak"); });

  mainMenu->display();
}


std::shared_ptr<ChoiceMenu> Program::createFullStockMenu(const std::string& filter_str) const {
  bool filterEnabled = false;
  if (!filter_str.empty()) { filterEnabled = true; }
  auto fullStockMenu = std::make_shared<ChoiceMenu>("Stock menu");
  for (const auto& product : products) {
    if (filterEnabled) {
      // name and filter to lowercase
      std::string lowercaseProductName = product->getName();
      std::ranges::transform(lowercaseProductName, lowercaseProductName.begin(), ::tolower);

      std::string lowercaseFilterStr = filter_str;
      std::ranges::transform(lowercaseFilterStr, lowercaseFilterStr.begin(), ::tolower);

      if (!lowercaseProductName.contains(lowercaseFilterStr)) {
        // Filtered out -> skip
        continue;
      }
    }

    fullStockMenu->addOption(product->getName(), createProductOptionHandler(product, fullStockMenu));
  }
  return fullStockMenu;
}


std::shared_ptr<SequentialMenu> Program::createFilterMenu() const {
  auto filterMenu = std::make_shared<SequentialMenu>("Filter objects");
  filterMenu->addCollection("Search query");
  return filterMenu;
}


std::function<void()> Program::createProductOptionHandler(const std::shared_ptr<Product>& product,
                                                          const std::shared_ptr<Menu>& parent) const {
  return [this, product, parent]() {
    const auto changeSpecificStockMenu = createChangeStockMenu(product);

    const std::string productInfo = buildProductInfo(product);

    const auto inspectMenu = std::make_shared<ChoiceMenu>("Inspect Product", nullptr);
    inspectMenu->addOption("Add stock", changeSpecificStockMenu);
    inspectMenu->setSuffixText(productInfo);
    inspectMenu->setParentMenu(parent);
    changeSpecificStockMenu->setParentMenu(parent);
    inspectMenu->display();
  };
}


std::shared_ptr<SequentialMenu> Program::createChangeStockMenu(const std::shared_ptr<Product>& product) const {
  // NOLINT(*-convert-member-functions-to-static)
  auto changeSpecificStockMenu = std::make_shared<SequentialMenu>("Stock menu");
  changeSpecificStockMenu->addCollection("Amount of stock to add");
  changeSpecificStockMenu->setHandler([product](const std::vector<std::string>& inputs) {
    product->setStockCount(product->getStockCount() + std::stoi(inputs[0]));
  });
  return changeSpecificStockMenu;
}


std::string Program::buildProductInfo(const std::shared_ptr<Product>& product) {
  std::string info =
      "Name: " + product->getName() +
      "\nManufacturer: " + product->getManufacturer() +
      "\nType: " + product->getTypeAsString() +
      "\nDiameter: " + std::to_string(product->getDiameter()) + " (inch)";

  if (product->instanceOf(TIRE)) {
    const auto tire = std::dynamic_pointer_cast<Tire>(product);
    info += "\nWidth: " + std::to_string(tire->getWidth()) + " (mm)"
        + "\nHeight: " + std::to_string(tire->getHeight()) + " (mm)"
        + "\nSpeed Index: " + std::string(1, tire->getSpeedIndex());
  } else if (product->instanceOf(RIM)) {
    const auto rim = std::dynamic_pointer_cast<Rim>(product);
    info += "\nWidth: " + std::to_string(rim->getWidth()) + " (mm)"
        + "\nColor: " + rim->getColor()
        + "\nMaterial: " + rim->getMaterialAsString();
  }

  info += "\nStock Count: " + std::to_string(product->getStockCount()) +
      "\nPrice (Individual): " + Product::convertCentsToReadable(product->getPriceIndividual()) +
      "\nPrice (Business): " + Product::convertCentsToReadable(product->getPriceBusiness());

  return info;
}


/**
 * @brief Handles full software initialization.
 */
void Program::init() {
  deserialize("../mem.json.bak");

  // FIXME: dev stuff, remove when serialization works
  users["GYLS"] = std::make_shared<User>("Gilles", ADMIN);
  users["ALSTY"] = std::make_shared<User>("Alec", EMPLOYEE);

  setupSession();
  initMenu();
}

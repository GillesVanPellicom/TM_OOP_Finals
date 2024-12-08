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
    if (users[cuid]->getPermissionLevel() == ADMIN) {
      permissionLevel = ADMIN;
    }


    // Be nice to the user
    std::cout << "Welcome " << users[cuid]->getUserName() << "\n" << std::endl;
    break;
  }
}


void Program::initMenu() {
  const auto stockMenu = std::make_shared<ChoiceMenu>("Stock menu");

  // const auto fullStockMenu = ;
  // const auto filteredStockMenu = ;

  stockMenu->addOption("Show full stock", createFullStockMenu());
  stockMenu->addOption("Show filtered stock", createStockFilterByQueryMenu(stockMenu));
  if (permissionLevel == ADMIN) {
    stockMenu->addOption("Add new product", createAddStockMenu());
  }

  const auto mainMenu = std::make_shared<ChoiceMenu>("Main menu");
  mainMenu->addOption("Stock", stockMenu);
  mainMenu->addOption("Save changes", [this] { serialize("../mem.json"); });

  mainMenu->display();
}


std::shared_ptr<ChoiceMenu> Program::createAddStockMenu() {
  const auto tireMenu = std::make_shared<SequentialMenu>("Add new tire");
  tireMenu->addCollection("Enter the tire name:");
  tireMenu->addCollection("Enter the manufacturer:");
  tireMenu->addCollection("Enter the diameter (in inches):");
  tireMenu->addCollection("Enter the stock count:");
  tireMenu->addCollection("Enter the individual price (in cents):");
  tireMenu->addCollection("Enter the business price (in cents):");
  tireMenu->addCollection("Enter the width (in mm):");
  tireMenu->addCollection("Enter the height (in mm):");
  tireMenu->addCollection("Enter the speed index (a single character):");

  // Set the handler to create and add the tire using collected inputs
  tireMenu->setHandler([this](const std::vector<std::string>& inputs) {
    try {
      // Parse inputs
      const std::string& name = inputs[0];
      const std::string& manufacturer = inputs[1];
      const std::uint32_t diameter = std::stoul(inputs[2]);
      const std::uint32_t stockCount = std::stoul(inputs[3]);
      const std::uint64_t priceIndividual = std::stoull(inputs[4]);
      const std::uint64_t priceBusiness = std::stoull(inputs[5]);
      const std::uint32_t width = std::stoul(inputs[6]);
      const std::uint32_t height = std::stoul(inputs[7]);

      // Ensure the speed index is a single character
      if (inputs[8].size() != 1) {
        throw std::invalid_argument("Speed index must be a single character.");
      }
      const char speedIndex = inputs[8][0];

      // Create and add the tire
      this->products.emplace_back(std::make_shared<Tire>(
        name,
        manufacturer,
        diameter,
        stockCount,
        priceIndividual,
        priceBusiness,
        width,
        height,
        speedIndex
      ));

      std::cout << "\033[1;32mTire added successfully!\033[0m\n";
      Menu::waitForAnyKey(false);
      initMenu();
    } catch (const std::exception& e) {
      std::cout << "\033[1;31mError: " << e.what() << "\033[0m\n";
      Menu::waitForAnyKey(false);
    }
  });


  // Implement Rim Menu
  const auto rimMenu = std::make_shared<SequentialMenu>("Add new rim");
  rimMenu->addCollection("Enter the rim name:");
  rimMenu->addCollection("Enter the manufacturer:");
  rimMenu->addCollection("Enter the diameter (in inches):");
  rimMenu->addCollection("Enter the stock count:");
  rimMenu->addCollection("Enter the individual price (in cents):");
  rimMenu->addCollection("Enter the business price (in cents):");
  rimMenu->addCollection("Enter the width (in mm):");
  rimMenu->addCollection("Enter the color:");
  rimMenu->addCollection("Enter the material (steel or aluminium):");

  rimMenu->setHandler([this](const std::vector<std::string>& inputs) {
    try {
      const std::string& name = inputs[0];
      const std::string& manufacturer = inputs[1];
      const std::uint32_t diameter = std::stoul(inputs[2]);
      const std::uint32_t stockCount = std::stoul(inputs[3]);
      const std::uint64_t priceIndividual = std::stoull(inputs[4]);
      const std::uint64_t priceBusiness = std::stoull(inputs[5]);
      const float width = std::stof(inputs[6]);
      const std::string& color = inputs[7];

      RimMaterial material;
      if (inputs[8] == "aluminium") {
        material = RimMaterial::ALUMINIUM;
      } else if (inputs[8] == "steel") {
        material = RimMaterial::STEEL;
      } else {
        throw std::invalid_argument("Invalid material type.");
      }

      this->products.emplace_back(std::make_shared<Rim>(
        name,
        manufacturer,
        diameter,
        stockCount,
        priceIndividual,
        priceBusiness,
        width,
        color,
        material
      ));

      std::cout << "\033[1;32mRim added successfully!\033[0m\n";
      Menu::waitForAnyKey(false);
      initMenu();
    } catch (const std::exception& e) {
      std::cout << "\033[1;31mError: " << e.what() << "\033[0m\n";
      Menu::waitForAnyKey(false);
    }
  });
  const auto _menu = std::make_shared<ChoiceMenu>("Add product");
  _menu->addOption("Add tire", tireMenu);
  _menu->addOption("Add rim", rimMenu);
  return _menu;
}


std::shared_ptr<ChoiceMenu> Program::createFullStockMenu(const std::string& filter_str) const {
  bool filterEnabled = false;
  if (!filter_str.empty()) { filterEnabled = true; }
  auto _menu = std::make_shared<ChoiceMenu>("Stock menu");
  for (const auto& product : products) {
    if (filterEnabled) {
      // name and filter to lowercase
      std::string lowercaseFilterStr = filter_str;
      std::ranges::transform(lowercaseFilterStr, lowercaseFilterStr.begin(), ::tolower);

      std::string lowercaseProductName = product->getName();
      std::ranges::transform(lowercaseProductName, lowercaseProductName.begin(), ::tolower);
      if (!lowercaseProductName.contains(lowercaseFilterStr)) {
        // Filtered out -> skip
        continue;
      }
    }

    _menu->addOption(product->getName(), createProductOptionHandler(product, _menu));
  }
  return _menu;
}


std::shared_ptr<SequentialMenu> Program::createStockFilterByQueryMenu(const std::shared_ptr<Menu>& parent) const {
    // Ensure _menu is successfully created.
    auto _menu = std::make_shared<SequentialMenu>("Filter objects");
    if (!_menu) {
        std::cerr << "Failed to create the filter menu!" << std::endl;
        return nullptr; // Exit early on failure.
    }

    _menu->setSuffixText(
        "Search format: X/<query>"
        "\n\tE.g. n/sport\n"
        "\nwhere X:"
        "\n\t- n: name"
        "\n\t- d: diameter"
        "\n\t- t: type\n"
    );
    _menu->addCollection("Search query");

    // Handler for user input
    _menu->setHandler([this, &parent, &_menu](const std::vector<std::string>& inputs) {
        if (inputs.empty()) {
            std::cerr << "Invalid input: No query provided." << std::endl;
            _menu->display();
            return;
        }

        const std::string& input = inputs[0];
        const auto delimiterPos = input.find('/');
        if (delimiterPos == std::string::npos || delimiterPos == 0 || delimiterPos == input.size() - 1) {
            std::cerr << "Invalid input format: Expected X/<query>." << std::endl;
            _menu->display();
            return;
        }

        const char filterType = input[0];
        std::string query = input.substr(delimiterPos + 1);

        // Convert query to lowercase
        std::ranges::transform(query.begin(), query.end(), query.begin(), ::tolower);

        // Initialize ChoiceMenu safely.
        const auto filteredFullStockMenu = std::make_shared<ChoiceMenu>("Filtered Stock Menu");
        if (!filteredFullStockMenu) {
            std::cerr << "Failed to create filtered stock menu!" << std::endl;
            return;
        }

        for (const auto& product : products) {
            std::string targetField;
            switch (filterType) {
                case 'n': // Name
                    targetField = product->getName();
                    break;
                case 'd': // Diameter
                    targetField = std::to_string(product->getDiameter());
                    break;
                case 't': // Type
                    targetField = product->getTypeAsString();
                    break;
                default:
                    std::cerr << "Invalid filter type: " << filterType << ". Please try again." << std::endl;
                    _menu->display();
                    return;
            }

            // Convert target field to lowercase
            std::ranges::transform(targetField.begin(), targetField.end(), targetField.begin(), ::tolower);

            if (targetField.contains(query)) {
                filteredFullStockMenu->addOption(product->getName(),
                                                 createProductOptionHandler(product, filteredFullStockMenu));
            }
        }

        // Safely set up parent-child relationships between menus.
        _menu->setParentMenu(filteredFullStockMenu);
        filteredFullStockMenu->setParentMenu(parent);

        // Display the menu after setup
        filteredFullStockMenu->display();
    });

    return _menu;
}


std::function<void()> Program::createProductOptionHandler(const std::shared_ptr<Product>& product,
                                                          const std::shared_ptr<Menu>& parent) const {
  return [this, product, parent]() {
    const auto changeStockMenu = createChangeStockMenu(product);

    const std::string productInfo = buildProductInfo(product);

    const auto inspectMenu = std::make_shared<ChoiceMenu>("Inspect Product", nullptr);
    inspectMenu->addOption("Add stock", changeStockMenu);
    // if (permissionLevel == ADMIN) {
    //   inspectMenu->addOption("Remove Product",
    //                          [this] {
    //                          });
    // }
    inspectMenu->setSuffixText(productInfo);
    inspectMenu->setParentMenu(parent);
    changeStockMenu->setParentMenu(parent);
    inspectMenu->display();
  };
}


std::shared_ptr<SequentialMenu> Program::createChangeStockMenu(const std::shared_ptr<Product>& product) const {
  // NOLINT(*-convert-member-functions-to-static)
  auto _menu = std::make_shared<SequentialMenu>("Stock menu");
  _menu->addCollection("Amount of stock to add");
  _menu->setHandler([product](const std::vector<std::string>& inputs) {
    product->setStockCount(product->getStockCount() + std::stoi(inputs[0]));
  });
  return _menu;
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
  deserialize("../mem.json");

  // FIXME: dev stuff, remove when serialization works
  users["GYLS"] = std::make_shared<User>("Gilles", ADMIN);
  users["ALSTY"] = std::make_shared<User>("Alec", EMPLOYEE);

  setupSession();
  initMenu();
}
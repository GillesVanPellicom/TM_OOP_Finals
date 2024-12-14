// ╔══════════════════════════════════════════════════════════════════════════════════╗
// ║ Name         : Program.cpp                                                      ║
// ║ Description  : Lorem ipsum dolor sit amet                                        ║
// ║                Lorem ipsum dolor sit amet                                        ║
// ║ Author(s)    : "Gilles Van pellicom" <r0997008@student.thomasmore.be>            ║
// ║ Date         : 2024/12/08                                                        ║
// ╚══════════════════════════════════════════════════════════════════════════════════╝

#include "Program.h"


template<typename T>
void Program::deserializeCollection(const nlohmann::json& j,
                                    const std::string& key,
                                    std::vector<std::shared_ptr<T> >& collection) {
  if (j.contains(key) && j[key].is_array()) {
    for (const auto& json : j[key]) {
      collection.emplace_back(std::make_shared<T>(json));
    }
  }
}


template<typename T>
void Program::serializeCollection(const std::vector<std::shared_ptr<T> >& collection,
                                  const std::string& key,
                                  nlohmann::json& j) const {
  if (!collection.empty()) {
    nlohmann::json a = nlohmann::json::array();
    for (const auto& item : collection) {
      a.push_back(item->serialize());
    }
    j[key] = a;
  }
}


void Program::serialize(const std::string& filePath) const {
  nlohmann::json j; // JSON root

  serializeCollection(products, "products", j);
  serializeCollection(customers, "customers", j);
  serializeCollection(invoices, "invoices", j);
  serializeCollection(companies, "companies", j);

  // Write the JSON to a file
  std::ofstream outputFile(filePath);
  if (!outputFile.is_open()) {
    std::cerr << "Could not open the file at " << filePath << std::endl;
    return;
  }

  outputFile << j.dump(4);
  outputFile.close();
  std::cout << "\033[1;32mSave complete.\033[0m\n" << std::endl;
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
    for (const auto& json : j["products"]) {
      if (auto type = json.at("type").get<std::string>();
        type == "tire") {
        products.emplace_back(std::make_shared<Tire>(json));
      } else if (type == "rim") {
        products.emplace_back(std::make_shared<Rim>(json));
      }
    }
  }
  // END Products

  deserializeCollection(j, "customers", customers);
  deserializeCollection(j, "invoices", invoices);
  deserializeCollection(j, "companies", companies);

  std::cout << "Loading from memory completed.\n" << std::endl;
}


void Program::setupSession() {
  while (true) {
    // get input
    std::string uid;
    std::cout << "\nPlease enter your UID:\033[0m\n> ";
    std::getline(std::cin, uid);
    std::cout << "\033[0m" << std::endl;

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
    std::cout << "Welcome " << users[cuid]->getUserName() << "\n\n" << std::endl;
    break;
  }
}


void Program::removeProduct(const std::shared_ptr<Product>& productToRemove) {
  std::erase_if(products,
                [productToRemove](const std::shared_ptr<Product>& product) {
                  return product == productToRemove;
                });
}


void Program::removeCustomer(const std::shared_ptr<Customer>& customerToRemove) {
  std::erase_if(customers,
                [customerToRemove](const std::shared_ptr<Customer>& customer) {
                  return customer == customerToRemove;
                });
}


void Program::initMenu() {
  const auto stockMenu = std::make_shared<ChoiceMenu>("Stock menu");
  stockMenu->init();
  stockMenu->addOption("Show all stock", createFullStockMenu());
  stockMenu->addOption("Filter stock", createStockFilterByQueryMenu(stockMenu));
  if (permissionLevel == ADMIN) {
    stockMenu->addOption("Add new product", createAddStockMenu());
  }


  const auto customerMenu = std::make_shared<ChoiceMenu>("Customers menu");
  customerMenu->init();
  customerMenu->addOption("Show all customers", createFullCustomerMenu());
  customerMenu->addOption("Filter customers", createCustomerFilterByQueryMenu(customerMenu));
  customerMenu->addOption("Add new customer", createAddCustomerMenu());
  customerMenu->addOption("Show all invoices", createFullInvoiceMenu());


  const auto mainMenu = std::make_shared<ChoiceMenu>("Main menu");
  mainMenu->init();
  mainMenu->addOption("Stock", stockMenu);
  mainMenu->addOption("Customers", customerMenu);
  mainMenu->addOption("Save changes", [this] { serialize("../mem.json"); });

  mainMenu->display();
}


std::shared_ptr<ChoiceMenu> Program::createAddInvoiceMenu(const std::shared_ptr<Customer>& c) {
  uint32_t businessDiscount = 0;

  // If company exists
  if (const auto company = getCompanyByUUID(c->getCompanyUUID())) {
    // get volume discount
    businessDiscount = company->getVolumeDiscount();
  }
  auto invoice = std::make_shared<Invoice>(c, businessDiscount);


  auto _menu = std::make_shared<ChoiceMenu>("Add new invoice item", nullptr, false);
  _menu->init();
  _menu->addOption("Done",
                   [this, invoice]() {
                     if (invoice->getPurchaseList().empty()) {
                       std::cout << "\033[1;32mInvoice creation successfully aborted.\033[0m\n" << std::endl;
                       initMenu();
                     }

                     for (const auto& purchase : invoice->getPurchaseList()) {
                       const auto& uuid = std::get<0>(purchase);
                       const uint32_t quantity = std::get<1>(purchase);
                       std::string name = std::get<3>(purchase);

                       if (const auto product = getProductByUUID(uuid)) {
                         // Process the product
                         if (!product->decrementStockCount(quantity)) {
                           std::cerr << "Failed to decrement stock for \"" << product->getName() << "\"" << std::endl;
                         } else {
                           std::cout << "Processed \"" << product->getName() << "\" successfully." << std::endl;
                         }
                       } else {
                         std::cerr << "Product with UUID \"" << uuid << "\" not found in inventory!" << std::endl;
                       }
                     }
                     std::cout << "\033[1;32mInvoice finalized. Stock successfully updated.\033[0m\n" << std::endl;
                     invoices.emplace_back(invoice);
                     initMenu();
                   });

  for (const auto& product : products) {
    _menu->addOption(product->getName(),
                     [this, product, _menu, invoice, c] {
                       // Create a menu for entering quantity
                       const auto qtyMenu = std::make_shared<SequentialMenu>("Add new invoice item");
                       qtyMenu->addCollection("Enter qty");

                       if (product->getType() == TIRE) {
                         qtyMenu->setSuffixText("1, 2 or 4 pcs.");
                       } else if (product->getType() == RIM) {
                         qtyMenu->setSuffixText("2 or 4 pcs.");
                       }

                       // Set the handler for qtyMenu to process the quantity
                       qtyMenu->setHandler([product, _menu, invoice, this, c](const std::vector<std::string>& inputs) {
                         const uint32_t output_qty = std::stoul(inputs[0]);

                         // Validate quantity based on product type
                         bool valid_qty = false;
                         if (product->getType() == TIRE) {
                           valid_qty = (output_qty == 1 || output_qty == 2 || output_qty == 4);
                         } else if (product->getType() == RIM) {
                           valid_qty = (output_qty == 2 || output_qty == 4);
                         }

                         if (!valid_qty) {
                           std::cerr <<
                               "Invalid quantity! For TIRE, valid quantities are 1, 2, or 4 pcs. For RIM, valid quantities are 2 or 4 pcs."
                               << std::endl;
                           Menu::waitForAnyKey(false);
                           initMenu();
                           return;
                         }

                         // Check if stock decrement is possible
                         if (!product->isDecrementPossible(output_qty)) {
                           std::cerr << "Not enough stock available" << std::endl;
                           Menu::waitForAnyKey(false);
                           initMenu();
                           return;
                         }

                         // Temporarily add the product and quantity to the invoice without altering stock
                         invoice->addPurchase(
                           product,
                           output_qty);

                         std::cout << "Product added to the invoice temporarily. Confirm or make further changes." <<
                             std::endl;
                         _menu->display();
                       });

                       qtyMenu->display(); // Display qtyMenu to allow quantity input
                     });
  }


  // Display the menu
  _menu->display();
  return _menu;
}


std::shared_ptr<Product> Program::getProductByUUID(const UUIDGen::UUID& uuid) {
  const auto it = std::ranges::find_if(products,
                                       [&uuid](const std::shared_ptr<Product>& product) {
                                         return product->getUUID() == uuid;
                                       });

  if (it != products.end()) {
    return *it; // Return the product if found
  }

  return nullptr; // Return null if no product matches the UUID
}


std::shared_ptr<Company> Program::getCompanyByUUID(const UUIDGen::UUID& uuid) {
  const auto it = std::ranges::find_if(companies,
                                       [&uuid](const std::shared_ptr<Company>& company) {
                                         return company->getUUID() == uuid;
                                       });

  if (it != companies.end()) {
    return *it; // Return the company if found
  }

  return nullptr; // Return null if no company matches the UUID
}


std::shared_ptr<SequentialMenu> Program::createEditCustomerMenu(const std::shared_ptr<Customer>& customer) {
  const auto _menu = std::make_shared<SequentialMenu>("Edit customer");

  // Add input fields, without pre-filling them with the existing values
  _menu->addCollection("Edit the first name [" + customer->getFirstName() + "]");
  _menu->addCollection("Edit the last name [" + customer->getLastName() + "]");
  _menu->addCollection("Edit the address [" + customer->getAddress() + "]");

  // Allow editing business status, based on the current customer's business status
  const std::string businessStatus = customer->isBusinessCustomer() ? "y" : "n";
  _menu->addCollection("Is business customer? (y/n) [" + businessStatus + "]");

  // If the customer is a business customer, allow editing VAT number and volume discount
  if (customer->isBusinessCustomer()) {
    _menu->addCollection("Edit the VAT number [" + customer->getAddress() + "]");
    _menu->addCollection("Edit volume discount [" + customer->getAddress() + "]");
  }

  // Set the handler to modify the existing customer using collected inputs
  _menu->setHandler([this, customer](const std::vector<std::string>& inputs) {
    try {
      char isBusiness;
      if (inputs[3].empty()) {
        // Retain old value
        isBusiness = customer->isBusinessCustomer() ? 'y' : 'n';
      } else {
        // New value handler
        if (inputs[3].size() != 1) {
          throw std::invalid_argument("Business customer must be a single character.");
        }
        isBusiness = static_cast<char>(std::tolower(static_cast<unsigned char>(inputs[3][0])));

        if (isBusiness != 'y' && isBusiness != 'n') {
          throw std::invalid_argument("Business customer must be y or n");
        }
      }


      // Set the new values to the customer or keep old ones
      customer->setFirstName(inputs[0].empty() ? customer->getFirstName() : inputs[0]);
      customer->setLastName(inputs[1].empty() ? customer->getLastName() : inputs[1]);
      customer->setAddress(inputs[2].empty() ? customer->getAddress() : inputs[2]);
      customer->setBusinessCustomer(inputs[3].empty() ? customer->isBusinessCustomer() : isBusiness == 'y');

      if (isBusiness == 'y') {
        // Set the business-specific details (if modified)
        if (const auto company = getCompanyByUUID(customer->getCompanyUUID()); !company) {
          // No company found, prompt the user to create one via another menu
          const auto companyMenu = std::make_shared<SequentialMenu>("Edit customer");

          // Collect VAT and volume discount
          companyMenu->setSuffixText("No prior associated company was found. Creating new company...");
          companyMenu->addCollection("Enter VAT number");
          companyMenu->addCollection("Enter volume discount");

          companyMenu->setHandler([this, customer](const std::vector<std::string>& companyInputs) {
            try {
              // Validate inputs
              if (companyInputs[0].empty() || companyInputs[1].empty()) {
                throw std::invalid_argument("VAT and volume discount must be provided.");
              }

              // Create a new company with the provided details
              const auto newCompany = std::make_shared<Company>(companyInputs[0], std::stoi(companyInputs[1]));

              // Add the new company to the system (assuming you have a collection of companies)
              this->companies.push_back(newCompany);

              // Assign the new company's UUID to the customer
              customer->setCompanyUUID(newCompany->getUUID());

              std::cout << "\033[1;32mCustomer edited and company created successfully!\033[0m\n\n";
              initMenu(); // Return to the main menu after company creation
            } catch (const std::exception& e) {
              std::cout << "\033[1;31mError: " << e.what() << "\033[0m\n\n";
            }
          });

          // Show the company creation menu
          companyMenu->display();
          return;
        } else {
          // If company exists, update VAT and volume discount
          company->setVat(inputs[4].empty() ? company->getVat() : inputs[4]);
          company->setVolumeDiscount(inputs[5].empty() ? company->getVolumeDiscount() : std::stoi(inputs[5]));
        }
      }

      std::cout << "\033[1;32mCustomer edited successfully!\033[0m\n\n";
      initMenu();
    } catch (const std::exception& e) {
      std::cout << "\033[1;31mError: " << e.what() << "\033[0m\n\n";
    }
  });

  return _menu;
}


std::shared_ptr<SequentialMenu> Program::createAddCustomerMenu() {
  const auto _menu = std::make_shared<SequentialMenu>("Add new customer");
  _menu->addCollection("Enter the first name");
  _menu->addCollection("Enter the last name");
  _menu->addCollection("Enter the address");
  _menu->addCollection("Is business customer? (y/n)");
  // Can't do if business because that variable isn't made yet.
  _menu->addCollection("Enter the VAT number (ignored if not a business customer)");
  _menu->addCollection("Enter volume discount (E.g. 10 = 10%, no decimals) (ignored if not a business customer)");


  // Set the handler to create and add the tire using collected inputs
  _menu->setHandler([this](const std::vector<std::string>& inputs) {
    try {
      if (inputs[3].size() != 1) {
        throw std::invalid_argument("Business customer must be a single character.");
      }
      const char isBusiness = static_cast<char>(std::tolower(static_cast<unsigned char>(inputs[3][0])));

      if (isBusiness != 'y' && isBusiness != 'n') {
        throw std::invalid_argument("Business customer must be y or n");
      }


      if (isBusiness == 'y') {
        this->companies.emplace_back(std::make_shared<Company>(
          inputs[4],
          stoi(inputs[5])));

        this->customers.emplace_back(std::make_shared<Customer>(
          inputs[0],
          inputs[1],
          inputs[2],
          true,
          this->companies.back()->getUUID()
        ));
      } else {
        // Create and add the customer
        this->customers.emplace_back(std::make_shared<Customer>(
          inputs[0],
          inputs[1],
          inputs[2],
          false
        ));
      }


      std::cout << "\033[1;32mCustomer added successfully!\033[0m\n\n";
      initMenu();
    } catch (const std::exception& e) {
      std::cout << "\033[1;31mError: " << e.what() << "\033[0m\n\n";
    }
  });

  return _menu;
}


std::shared_ptr<SequentialMenu> Program::createEditStockMenu(const std::shared_ptr<Product>& p) {
  const auto _menu = std::make_shared<SequentialMenu>("Edit Stock for Product");

  // Display current product details and allow the user to edit stock-related info
  _menu->addCollection("Edit the product name [" + p->getName() + "]");
  _menu->addCollection("Edit the manufacturer [" + p->getManufacturer() + "]");
  _menu->addCollection("Edit the stock count [" + std::to_string(p->getStockCount()) + "]");
  _menu->addCollection("Edit the individual price (in cents) [" + std::to_string(p->getPriceIndividual()) + "]");
  _menu->addCollection("Edit the business price (in cents) [" + std::to_string(p->getPriceBusiness()) + "]");
  _menu->addCollection("Edit the diameter (in inches) [" + std::to_string(p->getDiameter()) + "]");

  // Additional fields for Tire or Rim
  if (auto tire = std::dynamic_pointer_cast<Tire>(p)) {
    // Tire-specific fields
    _menu->addCollection("Edit the width (in mm) [" + std::to_string(tire->getWidth()) + "]");
    _menu->addCollection("Edit the height (in mm) [" + std::to_string(tire->getHeight()) + "]");
    _menu->addCollection("Edit the speed index (a single character) [" + std::string(1, tire->getSpeedIndex()) + "]");
  } else if (auto rim = std::dynamic_pointer_cast<Rim>(p)) {
    // Rim-specific fields
    _menu->addCollection("Edit the width (in mm) [" + std::to_string(rim->getWidth()) + "]");
    _menu->addCollection("Edit the color [" + rim->getColor() + "]");
    _menu->addCollection("Edit the material (steel or aluminium) [" + rim->getMaterialAsString() + "]");
  }

  // Set the handler to modify the stock and pricing information for the product
  _menu->setHandler([this, p](const std::vector<std::string>& inputs) {
    try {
      // Update basic product information
      p->setName(inputs[0].empty() ? p->getName() : inputs[0]);
      p->setManufacturer(inputs[1].empty() ? p->getManufacturer() : inputs[1]);
      p->setStockCount(inputs[2].empty() ? p->getStockCount() : std::stoul(inputs[2]));
      p->setPriceIndividual(inputs[3].empty() ? p->getPriceIndividual() : std::stoull(inputs[3]));
      p->setPriceBusiness(inputs[4].empty() ? p->getPriceBusiness() : std::stoull(inputs[4]));
      p->setDiameter(inputs[5].empty() ? p->getDiameter() : std::stoul(inputs[5]));

      // Handle tire-specific fields
      if (const auto tire = std::dynamic_pointer_cast<Tire>(p)) {
        tire->setWidth(inputs[6].empty() ? tire->getWidth() : std::stoul(inputs[6]));
        tire->setHeight(inputs[7].empty() ? tire->getHeight() : std::stoul(inputs[7]));
        tire->setSpeedIndex(inputs[8].empty() ? tire->getSpeedIndex() : inputs[8][0]);
      }

      // Handle rim-specific fields
      else if (const auto rim = std::dynamic_pointer_cast<Rim>(p)) {
        rim->setWidth(inputs[6].empty() ? rim->getWidth() : std::stof(inputs[6]));
        rim->setColor(inputs[7].empty() ? rim->getColor() : inputs[7]);

        // Transform the material input to lowercase
        std::string materialInput = inputs[8];
        std::ranges::transform(materialInput, materialInput.begin(), ::tolower);

        // Compare the lowercase input with the possible materials
        if (materialInput == "aluminium") {
          rim->setMaterial(RimMaterial::ALUMINIUM);
        } else if (materialInput == "steel") {
          rim->setMaterial(RimMaterial::STEEL);
        } else if (!materialInput.empty()) {
          throw std::invalid_argument("Invalid material type. Please use 'aluminium' or 'steel'.");
        }
      }

      std::cout << "\033[1;32mProduct updated successfully!\033[0m\n";
      initMenu();
    } catch (const std::exception& e) {
      std::cout << "\033[1;31mError: " << e.what() << "\033[0m\n";
      Menu::waitForAnyKey(false);
    }
  });

  return _menu;
}


std::shared_ptr<ChoiceMenu> Program::createAddStockMenu() {
  const auto tireMenu = std::make_shared<SequentialMenu>("Add new tire");
  tireMenu->addCollection("Enter the tire name");
  tireMenu->addCollection("Enter the manufacturer");
  tireMenu->addCollection("Enter the diameter (in inches)");
  tireMenu->addCollection("Enter the stock count");
  tireMenu->addCollection("Enter the individual price (in cents)");
  tireMenu->addCollection("Enter the business price (in cents)");
  tireMenu->addCollection("Enter the width (in mm)");
  tireMenu->addCollection("Enter the height (in mm)");
  tireMenu->addCollection("Enter the speed index (a single character)");

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
  rimMenu->addCollection("Enter the rim name");
  rimMenu->addCollection("Enter the manufacturer");
  rimMenu->addCollection("Enter the diameter (in inches)");
  rimMenu->addCollection("Enter the stock count");
  rimMenu->addCollection("Enter the individual price (in cents)");
  rimMenu->addCollection("Enter the business price (in cents)");
  rimMenu->addCollection("Enter the width (in mm)");
  rimMenu->addCollection("Enter the color");
  rimMenu->addCollection("Enter the material (steel or aluminium)");

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
  _menu->init();
  _menu->addOption("Add tire", tireMenu);
  _menu->addOption("Add rim", rimMenu);
  return _menu;
}


std::shared_ptr<ChoiceMenu> Program::createFullCustomerMenu() {
  auto _menu = std::make_shared<ChoiceMenu>("Customer menu");
  _menu->init();
  for (const auto& customer : customers) {
    const std::string& customerFullName = customer->getFirstName() + " " + customer->getLastName();
    _menu->addOption(customerFullName, createCustomerOptionHandler(customer, _menu));
  }
  return _menu;
}


std::shared_ptr<ChoiceMenu> Program::createFullStockMenu() {
  auto _menu = std::make_shared<ChoiceMenu>("Stock menu");
  _menu->init();
  for (const auto& product : products) {
    _menu->addOption(product->getName(), createProductOptionHandler(product, _menu));
  }
  return _menu;
}


std::shared_ptr<ChoiceMenu> Program::createFullInvoiceMenu(const std::shared_ptr<Customer>& c) const {
  auto _menu = std::make_shared<ChoiceMenu>("Invoice menu");
  _menu->init();
  for (const auto& invoice : invoices) {
    if (c != nullptr) {
      // Filter enabled
      if (c->getUUID() != invoice->getUUID()) {
        continue;
      }
    }

    _menu->addOption(invoice->getInvoiceName(), createInvoiceOptionHandler(invoice, _menu));
  }
  return _menu;
}


std::shared_ptr<SequentialMenu> Program::createCustomerFilterByQueryMenu(const std::shared_ptr<Menu>& parent) {
  // Ensure _menu is successfully created.
  auto _menu = std::make_shared<SequentialMenu>("Filter objects");
  if (!_menu) {
    std::cerr << "Failed to create the filter menu!" << std::endl;
    return nullptr; // Exit early on failure.
  }

  _menu->setSuffixText(
    "Search format: X/<query>"
    "\n\tE.g. n/john doe\n"
    "\nwhere X:"
    "\n\t- n: name"
    "\n\t- a: address"
  );
  _menu->addCollection("Search query");

  // Handler for user input
  _menu->setHandler([this, parent, _menu](const std::vector<std::string>& inputs) {
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
    const auto filteredFullCustomerMenu = std::make_shared<ChoiceMenu>("Filtered Stock Menu");
    filteredFullCustomerMenu->init();
    if (!filteredFullCustomerMenu) {
      std::cerr << "Failed to create filtered stock menu!" << std::endl;
      return;
    }

    for (const auto& customer : customers) {
      std::string targetField;
      const std::string customerFullName = customer->getFirstName() + " " + customer->getLastName();
      switch (filterType) {
        case 'n': // Name
          targetField = customerFullName;
          break;
        case 'a': // Address
          targetField = customer->getAddress();
          break;
        default:
          std::cerr << "Invalid filter type: " << filterType << ". Please try again." << std::endl;
          _menu->display();
          return;
      }

      // Convert target field to lowercase
      std::ranges::transform(targetField.begin(), targetField.end(), targetField.begin(), ::tolower);

      if (targetField.contains(query)) {
        filteredFullCustomerMenu->addOption(customerFullName,
                                            createCustomerOptionHandler(customer, filteredFullCustomerMenu));
      }
    }

    // Safely set up parent-child relationships between menus.
    _menu->setParentMenu(filteredFullCustomerMenu);
    filteredFullCustomerMenu->setParentMenu(parent);

    // Display the menu after setup
    filteredFullCustomerMenu->display();
  });

  return _menu;
}


std::function<void()> Program::createCustomerOptionHandler(const std::shared_ptr<Customer>& customer,
                                                           const std::shared_ptr<Menu>& parent) {
  return [this, customer, parent]() {
    const auto inspectMenu = std::make_shared<ChoiceMenu>("Inspect Customer", nullptr);
    inspectMenu->init();
    inspectMenu->addOption("Show invoices",
                           [this, customer, inspectMenu] {
                             const auto im = createFullInvoiceMenu(customer);
                             im->setParentMenu(inspectMenu);
                             im->display();
                           });
    inspectMenu->addOption("Create invoice",
                           [this, customer] {
                             createAddInvoiceMenu(customer);
                           });
    inspectMenu->addOption("Edit customer",
                           [this, customer] {
                             createEditCustomerMenu(customer)->display();
                             initMenu();
                           });
    if (permissionLevel == ADMIN) {
      inspectMenu->addOption("Remove Customer",
                             [this, customer]() {
                               removeCustomer(customer);
                               initMenu();
                             });
    }

    inspectMenu->setSuffixText(customer->buildCustomerInfo(getCompanyByUUID(customer->getCompanyUUID())));
    inspectMenu->setParentMenu(parent);
    inspectMenu->display();
  };
}


std::shared_ptr<SequentialMenu> Program::createStockFilterByQueryMenu(const std::shared_ptr<Menu>& parent) {
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
    "\n\t- t: type (tire, rim)"
  );
  _menu->addCollection("Search query");

  // Handler for user input
  _menu->setHandler([this, parent, _menu](const std::vector<std::string>& inputs) {
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
    filteredFullStockMenu->init();
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
                                                          const std::shared_ptr<Menu>& parent) {
  return [this, product, parent]() {
    const auto changeStockMenu = createChangeStockMenu(product);

    const std::string productInfo = product->buildProductInfo();

    const auto inspectMenu = std::make_shared<ChoiceMenu>("Inspect Product", nullptr);
    inspectMenu->init();
    inspectMenu->addOption("Add stock", changeStockMenu);
    inspectMenu->addOption("Edit Product",
                           [this, product]() {
                             createEditStockMenu(product)->display();
                             initMenu();
                           });
    if (permissionLevel == ADMIN) {
      inspectMenu->addOption("Remove Product",
                             [this, product]() {
                               removeProduct(product);
                               initMenu();
                             });
    }
    inspectMenu->setSuffixText(productInfo);
    inspectMenu->setParentMenu(parent);
    changeStockMenu->setParentMenu(parent);
    inspectMenu->display();
  };
}


std::function<void()> Program::createInvoiceOptionHandler(const std::shared_ptr<Invoice>& invoice,
                                                          const std::shared_ptr<Menu>& parent) {
  return [invoice, parent]() {
    const std::string invoiceInfo = invoice->buildInvoiceInfo();

    const auto inspectMenu = std::make_shared<ChoiceMenu>("Invoice Details", nullptr);
    inspectMenu->init();
    inspectMenu->setSuffixText(invoiceInfo);
    inspectMenu->setParentMenu(parent);
    inspectMenu->display();
  };
}


std::shared_ptr<SequentialMenu> Program::createChangeStockMenu(const std::shared_ptr<Product>& product) {
  auto _menu = std::make_shared<SequentialMenu>("Stock Menu");
  _menu->addCollection("Amount of stock to add");
  _menu->setHandler([product](const std::vector<std::string>& inputs) {
    product->setStockCount(product->getStockCount() + std::stoi(inputs[0]));
  });
  return _menu;
}


void Program::init() {
  deserialize("../mem.json");

  // FIXME: dev stuff, remove when serialization works
  users["GYLS"] = std::make_shared<User>("Gilles", ADMIN);
  users["ALSTY"] = std::make_shared<User>("Alec", EMPLOYEE);

  setupSession();
  initMenu();
}

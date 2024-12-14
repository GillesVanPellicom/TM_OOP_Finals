// ╔══════════════════════════════════════════════════════════════════════════════════╗
// ║ Name         : Program.h                                                      ║
// ║ Description  : Lorem ipsum dolor sit amet                                        ║
// ║                Lorem ipsum dolor sit amet                                        ║
// ║ Author(s)    : "Gilles Van pellicom" <r0997008@student.thomasmore.be>            ║
// ║ Date         : 2024/12/08                                                        ║
// ╚══════════════════════════════════════════════════════════════════════════════════╝

#ifndef PROGRAM_H
#define PROGRAM_H

// STL
#include <iostream>
#include <fstream>

// Local
#include "object/product/tire/Tire.h"
#include "object/product/rim/Rim.h"
#include "menu/ChoiceMenu.hpp"
#include "menu/SequentialMenu.hpp"
#include "object/customer/Customer.h"
#include "object/user/User.h"
#include "object/invoice/Invoice.h"
#include "object/company/Company.h"
#include "object/invoice/Invoice.h"


class Program {
  private:
    std::unordered_map<std::string, std::shared_ptr<User> > users;
    std::vector<std::shared_ptr<Product> > products;
    std::vector<std::shared_ptr<Customer> > customers;
    std::vector<std::shared_ptr<Invoice> > invoices;
    std::vector<std::shared_ptr<Company> > companies;
    std::string cuid; // Current User ID
    UserPermissionLevel permissionLevel = EMPLOYEE; // Default is EMPLOYEE

    /**
   * @brief Saves contents of program to memory
   *
   * @param filePath Path to json save file
   */
    void serialize(const std::string& filePath) const;


    /**
   * @brief Reads in contents of program from memory
   *
   * @param filePath Path to json save file
   */
    void deserialize(const std::string& filePath);


    void removeProduct(const std::shared_ptr<Product>& productToRemove);
    void removeCustomer(const std::shared_ptr<Customer>& customerToRemove);
    /**
   * @brief Asks user for their UID and sets up session permissions accordingly
   */
    void setupSession();


    /**
    * @brief Entrypoint for the menu tree. Loads main menu.
    */
    void initMenu();


    // Following are all menu-related functions which should only be called internally.
    // They are functions purely for modularity, readability and separation of concern.
    // Commenting all of these is not useful.

    std::function<void()> createProductOptionHandler(const std::shared_ptr<Product>& product,
                                                     const std::shared_ptr<Menu>& parent);
    static std::function<void()> createInvoiceOptionHandler(const std::shared_ptr<Invoice>& invoice,
                                                            const std::shared_ptr<Menu>& parent);
    std::function<void()> createCustomerOptionHandler(const std::shared_ptr<Customer>& customer,
                                                      const std::shared_ptr<Menu>& parent);
    std::shared_ptr<ChoiceMenu> createAddInvoiceMenu(const std::shared_ptr<Customer>& c);
    std::shared_ptr<SequentialMenu> createAddCustomerMenu();
    std::shared_ptr<ChoiceMenu> createAddStockMenu();
    std::shared_ptr<ChoiceMenu> createFullCustomerMenu();
    std::shared_ptr<ChoiceMenu> createFullInvoiceMenu(const std::shared_ptr<Customer>& c = nullptr) const;
    std::shared_ptr<ChoiceMenu> createFullStockMenu();
    std::shared_ptr<SequentialMenu> createCustomerFilterByQueryMenu(const std::shared_ptr<Menu>& parent);
    std::shared_ptr<SequentialMenu> createStockFilterByQueryMenu(const std::shared_ptr<Menu>& parent);
    static std::shared_ptr<SequentialMenu> createChangeStockMenu(const std::shared_ptr<Product>& product);
    std::shared_ptr<SequentialMenu> createEditCustomerMenu(const std::shared_ptr<Customer>& customer);


    /**
     * @brief Helper function used to locate a Product by UUID
     * @param uuid UUID from the Product object to be located
     * @return shared_ptr to an object of type Product
     */
    [[nodiscard]] std::shared_ptr<Product> getProductByUUID(const UUIDGen::UUID& uuid);


    /**
    * @brief Helper function used to locate a Company by UUID
    * @param uuid UUID from the Company object to be located
    * @return shared_ptr to an object of type Company
    */
    [[nodiscard]] std::shared_ptr<Company> getCompanyByUUID(const UUIDGen::UUID& uuid);


    /**
    * @brief Deserializes a JSON collection into a vector of shared pointers to type T.
    *
    * @param j The JSON object containing the data.
    * @param key The key in the JSON object to retrieve the collection from.
    * @param collection The vector to populate with deserialized elements.
    * @tparam T The type of the elements in the collection.
    */
    template<typename T>
    void deserializeCollection(const nlohmann::json& j,
                               const std::string& key,
                               std::vector<std::shared_ptr<T> >& collection);


    /**
    * @brief Serializes a vector of shared pointers to type T into a JSON object.
    * @note Object in collection must have a serialize() function which outputs a nlohmann JSON object.
    * @param collection The vector of shared pointers to serialize.
    * @param key The key to use in the JSON object.
    * @param j The JSON object to populate with the serialized data.
    * @tparam T The type of the elements in the collection.
    */
    template<typename T>
    void serializeCollection(const std::vector<std::shared_ptr<T> >& collection,
                             const std::string& key,
                             nlohmann::json& j) const;

  public:
    /**
    * @brief Handles full software initialization.
    */
    void init();
};


#endif //PROGRAM_H

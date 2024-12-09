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

// Local
#include <fstream>

#include "object/product/tire/Tire.h"
#include "object/product/rim/Rim.h"
#include "menu/ChoiceMenu.hpp"
#include "menu/SequentialMenu.hpp"
#include "object/customer/Customer.h"
#include "object/user/User.h"

class Program {
  private:
    std::unordered_map<std::string, std::shared_ptr<User> > users;
    std::vector<std::shared_ptr<Product> > products;
    std::vector<std::shared_ptr<Customer> > customers;
    std::string cuid;
    UserPermissionLevel permissionLevel = EMPLOYEE;

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
      // They are functions purely for modularity and separation of concern.
      // I will not comment all of these.

      std::function<void()> createProductOptionHandler(const std::shared_ptr<Product>& product,
                                                       const std::shared_ptr<Menu>& parent);
      std::function<void()> createCustomerOptionHandler(const std::shared_ptr<Customer>& customer,
                                                        const std::shared_ptr<Menu>& parent);
      std::shared_ptr<SequentialMenu> createAddCustomerMenu();
      std::shared_ptr<ChoiceMenu> createAddStockMenu();
      std::shared_ptr<ChoiceMenu> createFullCustomerMenu();
      std::shared_ptr<ChoiceMenu> createFullStockMenu();
      std::shared_ptr<SequentialMenu> createCustomerFilterByQueryMenu(const std::shared_ptr<Menu>& parent);
      std::shared_ptr<SequentialMenu> createStockFilterByQueryMenu(const std::shared_ptr<Menu>& parent);
      std::shared_ptr<SequentialMenu> createChangeStockMenu(const std::shared_ptr<Product>& product);
      static std::string buildProductInfo(const std::shared_ptr<Product>& product);

      public
      :
      /**
      * @brief Handles full software initialization.
      */
      void init();
    };


#endif //PROGRAM_H

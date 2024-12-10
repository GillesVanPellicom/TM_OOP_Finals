// ╔══════════════════════════════════════════════════════════════════════════════════╗
// ║ Name         : SequentialMenu.hpp                                                ║
// ║ Description  : Implementation and definition of the class SequentialMenu         ║
// ║ Inherit(s)   : Menu.hpp                                                          ║
// ║ Author(s)    : "Gilles Van pellicom" <r0997008@student.thomasmore.be>            ║
// ║ Date         : 2024/12/07                                                        ║
// ╚══════════════════════════════════════════════════════════════════════════════════╝

#ifndef SEQUENTIAL_MENU_HPP
#define SEQUENTIAL_MENU_HPP

// STD
#include <iostream>
#include <string>
#include <functional>
#include <vector>

// Local
#include "Menu.hpp"


/**
 * @brief A menu class that prompts users sequentially for input and processes the collected data.
 *
 * The `SequentialMenu` class extends the `Menu` class and adds functionality for sequentially collecting user inputs.
 * Each input is prompted by a specified prompt, and the collected inputs are stored and processed using a handler function.
 * After the inputs are collected, the handler function is called with the collected data.
 * Once the handler processes the input, the menu will display the parent menu.
 *
 * Derived from `Menu`, this class is useful when you need to collect a series of inputs from the user and perform
 * operations based on those inputs.
 *
 * @see Menu
 */
class SequentialMenu final : public Menu {
  private:
    // ╔════════════════════════════════════════╗
    // ║              Attributes                ║
    // ╚════════════════════════════════════════╝

    // Collection prompts in order
    std::vector<std::string> inputPrompts;
    // Collection inputs in order
    std::vector<std::string> collectedInputs;


    // ╔════════════════════════════════════════╗
    // ║           Private Methods              ║
    // ╚════════════════════════════════════════╝

    /**
     * Helper function to collect input
     * @param prompt Prompt to be shown to user
     */
    void collect(const std::string& prompt) {
      std::cout << "\n" << prompt << ": \n> ";

      std::string var;
      std::getline(std::cin, var);
      collectedInputs.emplace_back(var);
    }


    /**
     * @brief Handler function to process the collected inputs.
     */
    std::function<void(const std::vector<std::string>&)> handler;


    // ╔════════════════════════════════════════╗
    // ║            Public Methods              ║
    // ╚════════════════════════════════════════╝

  public:
    /**
     * @brief Adds an input collection stage to the menu
     *
     * @param prompt Prompt to be shown to user
     */
    void addCollection(const std::string& prompt) {
      inputPrompts.emplace_back(prompt);
    }


    /**
     * @brief Displays the menu and sequentially asks for inputs.
     */
    void display() override {
      collectedInputs.clear();

      std::cout << "\033[1;33m[" << menuName << "]\033[0m" << suffixText;
      // Clear input buffer
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      // Loop through the input actions and display prompts
      for (auto& prompt : inputPrompts) {
        collect(prompt);
      }

      std::cout << std::endl;

      // After collecting inputs, call the handler function
      if (handler) { handler(collectedInputs); }

      // waitForAnyKey(false);
      // Go one menu up
      getParentMenu()->display();
    }


    // ╔════════════════════════════════════════╗
    // ║           Getters & Setters            ║
    // ╚════════════════════════════════════════╝

    /**
     * @brief Set the handler function to process collected inputs.
     *
     * The handler function is called when input collection is complete to handle said input.
     * A vector of input strings is passed as argument.
     */
    void setHandler(std::function<void(const std::vector<std::string>&)> new_handler) {
      handler = std::move(new_handler);
    }


    // ╔════════════════════════════════════════╗
    // ║             Constructors               ║
    // ╚════════════════════════════════════════╝

    explicit SequentialMenu(const std::string& menu_name, const std::shared_ptr<Menu>& parent_menu = nullptr)
      : Menu(menu_name, parent_menu) {
    }
};

#endif // SEQUENTIAL_MENU_HPP

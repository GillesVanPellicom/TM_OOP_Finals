#ifndef SEQUENTIAL_MENU_HPP
#define SEQUENTIAL_MENU_HPP

#include <iostream>
#include <string>
#include <functional>
#include <vector>


#include "Menu.hpp"

class SequentialMenu final : public Menu {
  private:
    // Vector to hold the prompts and the corresponding input actions
    std::vector<std::string> inputPrompts;
    // Vector to store the user input values
    std::vector<std::string> collectedInputs;


    // ╔════════════════════════════════════════╗
    // ║               functions                ║
    // ╚════════════════════════════════════════╝

    /**
     * Helper function to collect input
     * @param prompt Prompt to be shown to user
     */
    void collect(const std::string& prompt) {
      std::cout << "\n"<< prompt << "\n> ";

      std::string var;
      std::getline(std::cin, var);
      collectedInputs.emplace_back(var);
    }

    /**
      * @brief Handler function to process the collected inputs.
      */
    std::function<void(const std::vector<std::string>&)> handler;

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
      parentMenu->display();
    }

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

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

    /**
     * Helper function to collect input
     * @param prompt Prompt to be shown to user
     */
    void collect(const std::string& prompt) {
      std::cout << prompt << "\n> ";

      std::string var;
      std::getline(std::cin, var);
      collectedInputs.emplace_back(var);
    }

    /**
   * @brief Handler function to process the collected inputs.
   */
    std::function<void(const std::vector<std::string>&)> handler;

  public:
    explicit SequentialMenu(const std::string& menu_name)
      : Menu(menu_name) {
    }


    /**
     * @brief Adds a collection stage to the menu
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
      std::cout << "\n\033[1;33m[" << menuName << "]\033[0m\n";
      // Clear input buffer
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      // Loop through the input actions and display prompts
      for (auto& prompt : inputPrompts) {
        collect(prompt);
      }

      // After collecting inputs, call the handler function
      if (handler) { handler(collectedInputs); }

      waitForAnyKey(false);
      // Go one menu up
      parentMenu->display();
    }

    /**
     * @brief Set the handler function to process collected inputs.
     */
    void setHandler(std::function<void(const std::vector<std::string>&)> new_handler) {
      handler = std::move(new_handler);
    }
};

#endif // SEQUENTIAL_MENU_HPP

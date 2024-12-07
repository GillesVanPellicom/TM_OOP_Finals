#ifndef CHOICE_MENU_H
#define CHOICE_MENU_H

#include "Menu.hpp"
#include <memory>  // For std::shared_ptr
#include <utility>

class ChoiceMenu final : public Menu {
  private:
    std::map<int, MenuEntry> options;
    int optionCount = 1;
    bool shouldExit = false;

  public:
    // Constructor accepts an optional parent menu
    explicit ChoiceMenu(const std::string& menu_name, const std::shared_ptr<Menu>& parent_menu = nullptr)
      : Menu(menu_name, parent_menu) {
      // Initialize exit option
      addOption("Exit",
                [this]() {
                  shouldExit = true;
                });
    }


    void addOption(std::string description, const std::function<void()>& action) {
      options[optionCount++] = {std::move(description), action};
    }

    void addSubMenu(std::string description, Menu& submenu) {
      options[optionCount++] = {
        std::move(description), [&submenu, this]() {
          const auto parent = std::shared_ptr<Menu>(
            this,
            [](Menu*) {
            }); // Preventing ownership transfer
          submenu.setParentMenu(parent);
          submenu.display();
        }
      };
    }


    // Displays the menu options
    [[noreturn]] void display() override {
      int choice;
      while (true) {
        std::cout << "\n\033[1;33m[" << menuName << "]\033[0m\nChoose an option:\n";
        for (const auto& [i, entry] : options) {
          std::cout << "\t\033[1;33m" << i << ".\033[0m " << entry.description << "\n";
        }

        // User input
        std::cout << "\nEnter your choice: \033[0m\n> ";
        std::cin >> choice;
        std::cout << "\033[0m\n" << std::endl;

        // If wrong input, continue
        if (std::cin.fail() || !options.contains(choice)) {
          // Clear the error flag and ignore the incorrect input in the buffer
          std::cin.clear(); // Clear the fail state
          std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore incorrect input

          std::cout << "\033[1;31mInvalid option. Please try again.\033[0m\n";

          continue;
        }

        // Perform action
        options[choice].action();

        if (shouldExit) {
          if (parentMenu) {
            parentMenu->display(); // Return to the parent menu
          } else {
            std::cout << "Goodbye.\n";
            exit(0);
          }
        }
        waitForAnyKey(true);
      }
    }
};

#endif //CHOICE_MENU_H

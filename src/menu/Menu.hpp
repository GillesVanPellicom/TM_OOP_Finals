// ╔══════════════════════════════════════════════════════════════════════════════════╗
// ║ Name         : Menu.h                                                      ║
// ║ Description  : Lorem ipsum dolor sit amet                                        ║
// ║                Lorem ipsum dolor sit amet                                        ║
// ║ Author(s)    : "Gilles Van pellicom" <r0997008@student.thomasmore.be>            ║
// ║ Date         : 2024/12/07                                                        ║                
// ║ Version      : 1.0                                                               ║
// ║ License      : GPL-3.0                                                           ║
// ╚══════════════════════════════════════════════════════════════════════════════════╝

#ifndef MENU_H
#define MENU_H


#include <iostream>
#include <functional>
#include <map>
#include <utility>

class Menu {
  public:
    /**
     *
     */
    struct MenuOption {
      std::string description;
      std::function<void()> action;
    };

  private:
    /**
     *
     */
    std::map<int, MenuOption> options;
    std::string menuName;

  public:
    explicit Menu(std::string menu_name)
      : menuName(std::move(menu_name)) {
    }

    /**
     * @brief adds an action to the menu options
     *
     * @param optionNumber number to choose this option
     * @param description description of the option
     * @param action function to be called
     */
    void addOption(const int optionNumber, std::string description, const std::function<void()>& action) {
      options[optionNumber] = {std::move(description), action};
    }

    /**
     * @brief adds a sub menu to the menu options
     *
     * @param optionNumber number to choose this option
     * @param description description of the option
     * @param submenu reference to another menu object
     */
    void addSubMenu(const int optionNumber, std::string description, Menu& submenu) {
      options[optionNumber] = {std::move(description), [&submenu]() { submenu.display(); }};
    }

    /**
     * @brief Displays the menu
     *
     * Should only be called after the menu is constructed.
     */
    [[noreturn]] void display() {
      int choice;
      while (true) {
        std::cout << "\n\033[1;33m[" << menuName << "]\033[0m\nChoose an option:\n";
        for (const auto& [o1, o2] : options) {
          std::cout << "\t\033[1;33m" << o1 << ".\033[0m " << o2.description << "\n";
        }

        // User input
        std::cout << "\nEnter your choice: \033[0m\n";
        std::cin >> choice;
        std::cout << "\033[0m\n" << std::endl;

        // Check if option exists
        if (std::cin.fail() || !options.contains(choice)) {
          // Clear the error flag and ignore the incorrect input in the buffer
          std::cin.clear(); // Clear the fail state
          std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore incorrect input

          std::cout << "\033[1;31mInvalid option. Please try again.\033[0m\n";

          continue;
        }
        options[choice].action();
      }
    }
};


#endif //MENU_H

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
#include <vector>
#include <functional>
#include <map>

class Menu {
public:
    // Menu option struct
    struct MenuOption {
        std::string description;       // Text displayed for the option
        std::function<void()> action;  // Action to execute when selected
    };

    // Add a new option to the current menu
    void addOption(const int optionNumber, const std::string& description, const std::function<void()>& action) {
        options[optionNumber] = {description, action};
    }

    // Add a submenu to the menu
    void addSubMenu(const int optionNumber, const std::string& description, Menu& submenu) {
        options[optionNumber] = {description, [&submenu]() { submenu.display(); }};
    }

    // Display the menu and prompt for user input
    [[noreturn]] void display() {
        int choice;
        while (true) {
            std::cout << "\nChoose an option:\n";
            for (const auto& option : options) {
                std::cout << option.first << ". " << option.second.description << "\n";
            }

            std::cout << "Enter your choice: ";
            std::cin >> choice;

            if (options.contains(choice)) {
                options[choice].action();
            } else {
                std::cout << "Invalid option. Please try again.\n";
            }
        }
    }

private:
    std::map<int, MenuOption> options; // Stores menu options
};





#endif //MENU_H

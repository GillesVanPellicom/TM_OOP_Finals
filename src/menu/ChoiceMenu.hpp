#ifndef CHOICE_MENU_H
#define CHOICE_MENU_H

#include <memory>

#include "Menu.hpp"

class ChoiceMenu final : public Menu {
  private:
    std::map<int, MenuEntry> options;
    int optionCount = 1;
    bool shouldExit = false;
    bool isInitialized = false;
    std::shared_ptr<Menu> ptr_this;

  public:
    // ╔════════════════════════════════════════╗
    // ║             Constructors               ║
    // ╚════════════════════════════════════════╝

    explicit ChoiceMenu(const std::string& menu_name,
                        const std::shared_ptr<Menu>& parent_menu = nullptr,
                        const bool exitOption = true)
      : Menu(menu_name, parent_menu) {
      // Initialize exit option
      if (exitOption) {
        addOption("Exit", [this]() { shouldExit = true; });
      }
    }


    // ╔════════════════════════════════════════╗
    // ║               functions                ║
    // ╚════════════════════════════════════════╝

    /**
     * @brief adds a menu option which triggers a function
     * @param description description of the option
     * @param action function to be called
     */
    void addOption(std::string description, const std::function<void()>& action) {
//        if (!isInitialized) {
//            isInitialized = true;
//init();
//        }
      options[optionCount++] = {std::move(description), action};
    }

    /**
     * @brief adds a menu option which opens a sub-menu
     * @param description description of the option
     * @param submenu menu to be opened
     */
    void addOption(std::string description, const std::shared_ptr<Menu>& submenu) {
        if (!isInitialized) {
            isInitialized = true;
init();
        }

      submenu->setParentMenu(ptr_this);
      options[optionCount++] = {
        std::move(description), [submenu]() {
          submenu->display();
        }
      };
    }

    void init() {
        // Since this class is derived from Menu and Menu is the implementing cl
        ptr_this = shared_from_this();
    }


    /**
     * @brief Displays the menu and asks for a single choice.
     */
    [[noreturn]] void display() override {
      int choice;
      while (true) {
        std::cout << "\033[1;33m[" << menuName << "]\033[0m" << suffixText << "\n\nChoose an option:\n";
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
          waitForAnyKey(false);
          continue;
        }

        // Perform action
        options[choice].action();

        if (shouldExit) {
          if (parentMenu.lock()) {
            getParentMenu()->display(); // Return to the parent menu
          } else {
            std::cout << "Goodbye.\n";
            exit(0);
          }
        }
      }
    }
};

#endif //CHOICE_MENU_H

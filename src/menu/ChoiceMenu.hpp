// ╔══════════════════════════════════════════════════════════════════════════════════╗
// ║ Name         : ChoiceMenu.hpp                                                    ║
// ║ Description  : Implementation and definition of the class ChoiceMenu             ║
// ║ Inherit(s)   : Menu.hpp                                                          ║
// ║ Author(s)    : "Gilles Van pellicom" <r0997008@student.thomasmore.be>            ║
// ║ Date         : 2024/12/07                                                        ║
// ╚══════════════════════════════════════════════════════════════════════════════════╝

#ifndef CHOICE_MENU_H
#define CHOICE_MENU_H

// Local
#include "Menu.hpp"


/**
 * @brief A menu class that presents a list of choices to the user, each associated with an action or a submenu.
 *
 * The `ChoiceMenu` class extends the `Menu` class and allows users to select an option from a list of choices.
 * Each option is associated with an action, which is executed when the option is selected. The menu can also contain
 * submenus that will be displayed when their corresponding option is selected. An option "exit" is added by default
 * in the first position, but can be disabled. Once an option is selected and its corresponding action is executed, the menu
 * will either display the next menu (if the "Exit" option is not selected) or return to the parent menu.
 *
 * This class provides functionality for handling invalid input and ensures that the user can select only valid options.
 *
 * @see Menu
 */
class ChoiceMenu final : public Menu {
  private:
    // ╔════════════════════════════════════════╗
    // ║              Attributes                ║
    // ╚════════════════════════════════════════╝

    /**
      * @brief Data-structure used to keep track of menu option to action relations
      */
  struct MenuEntry {
    std::string description;
    std::function<void()> action;
  };

    std::map<int, MenuEntry> options;
    int optionCount = 1;
    bool shouldExit = false;
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
    // ║            Public Methods              ║
    // ╚════════════════════════════════════════╝

    /**
     * @brief Adds a menu option which calls a function on trigger.
     * @param description description of the option
     * @param action function to be called
     */
    void addOption(std::string description, const std::function<void()>& action) {
      options[optionCount++] = {std::move(description), action};
    }


    /**
     * @brief Adds a menu option which opens a sub-menu on trigger.
     * @param description description of the option
     * @param submenu menu to be opened
     */
    void addOption(std::string description, const std::shared_ptr<Menu>& submenu) {
      submenu->setParentMenu(ptr_this);
      options[optionCount++] = {
        std::move(description), [submenu]() {
          submenu->display();
        }
      };
    }


    /**
      * @brief Initializes ChoiceMenu. Parent functionality does not work until this function is called.
      * Shared_from_this() may only be called once an object is managed by a shared_ptr,
      * which ChoiceMenu is not at time of construction.
      * If I don't filter out the shared_from_this() to this init() step it'll throw a weak_ptr() tantrum.
      *
      * Using a factory pattern to manage this would have been more practical,
      * (and probably more practical in general compared to just initializing classes)
      * but I've only found this particular tidbit out at the end of development...
      *
      * Calling init() manually for every ChoiceMenu it is.
      */
    void init() {
      ptr_this = shared_from_this();
    }


    /**
     * @brief Displays the menu and asks for a single numerical choice as per the given options.
     */
    [[noreturn]] void display() override {
      int choice;
      while (true) {
        std::cout << "\033[1;33m[" << menuName << "]\033[0m" << suffixText << "\nChoose an option:\n";
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

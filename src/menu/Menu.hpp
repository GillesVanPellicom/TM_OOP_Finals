// ╔══════════════════════════════════════════════════════════════════════════════════╗
// ║ Name         : Menu.hpp                                                          ║
// ║ Description  : Implementation and definition of the abstract class Menu          ║
// ║ Child(ren)   : ChoiceMenu.hpp, SequentialMenu.hpp                                ║
// ║ Author(s)    : "Gilles Van pellicom" <r0997008@student.thomasmore.be>            ║
// ║ Date         : 2024/12/07                                                        ║
// ╚══════════════════════════════════════════════════════════════════════════════════╝

#ifndef MENU_HPP
#define MENU_HPP

// Local
#include <iostream>
#include <functional>


/**
 * @brief Abstract base class representing a menu system.
 *
 * The `Menu` class serves as a base class for creating menus that can be part of a hierarchical menu system.
 * Each menu has a name, an optional parent menu, and a suffix text. The menu allows the display of
 * options and the triggering of actions based on user input.
 *
 * Derived classes should implement the `display()` method to show the menu content and handle user interaction.
 * The `MenuEntry` structure defines an entry in the menu, which includes a description and an associated action.
 *
 * @note Derived classes include `ChoiceMenu` and `SequentialMenu`, which extend the functionality of the menu system.
 *
 * @see ChoiceMenu, SequentialMenu
 */
class Menu : public std::enable_shared_from_this<Menu> {
  // ╔════════════════════════════════════════╗
  // ║              Attributes                ║
  // ╚════════════════════════════════════════╝
  protected:
    std::string menuName;
    std::weak_ptr<Menu> parentMenu;
    std::string suffixText;


  public:
    // ╔════════════════════════════════════════╗
    // ║            Public Methods              ║
    // ╚════════════════════════════════════════╝

    /**
     * @brief Prompts the user to press any key and waits until action.
     * @param ignore Should an extra cin.ignore() be called
     */
    static void waitForAnyKey(const bool ignore) {
      std::cout << "Press any key to continue...\n";
      if (ignore) {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }
      std::cin.get();
    }

    /**
     * @brief Abstract display() for polymorphism.
     */
    virtual void display() = 0;


    // ╔════════════════════════════════════════╗
    // ║             Constructors               ║
    // ╚════════════════════════════════════════╝

    explicit Menu(std::string menu_name, const std::shared_ptr<Menu>& parent_menu = nullptr)
      : menuName(std::move(menu_name)),
        parentMenu(parent_menu) {
    }


    // ╔════════════════════════════════════════╗
    // ║           Getters & Setters            ║
    // ╚════════════════════════════════════════╝

    [[nodiscard]] std::string getMenuName() const {
      return menuName;
    }
    void setMenuName(std::string menu_name) {
      menuName = std::move(menu_name);
    }


    [[nodiscard]] std::shared_ptr<Menu> getParentMenu() const {
      return parentMenu.lock(); // Returns nullptr if parent expired
    }
    void setParentMenu(const std::shared_ptr<Menu>& parent_menu) {
      parentMenu = parent_menu;
    }


    [[nodiscard]] std::string getSuffixText() const {
      return suffixText;
    }
    void setSuffixText(std::string suffix_text) {
      suffix_text = "\n" + suffix_text + "\n";
      suffixText = std::move(suffix_text);
    }

    // ╔════════════════════════════════════════╗
    // ║              Destructors               ║
    // ╚════════════════════════════════════════╝

    virtual ~Menu() = default;
};


#endif //MENU_HPP

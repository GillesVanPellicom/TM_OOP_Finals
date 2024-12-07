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

class Menu {
  protected:
    std::string menuName;
    std::shared_ptr<Menu> parentMenu;

  public:
    struct MenuEntry {
      std::string description;
      std::function<void()> action;
    };


    /**
     * @brief waits and prompts the user to press any key
     * @param ignore Should a cin.ignore() be called
     */
    static void waitForAnyKey(bool ignore) {
      std::cout << "Press any key to continue...\n";
      if (ignore) {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }
      std::cin.get();
    }


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
      return parentMenu;
    }
    void setParentMenu(const std::shared_ptr<Menu>& parent_menu) {
      parentMenu = parent_menu;
    }

    // Misc
    virtual ~Menu() = default;
    virtual void display() = 0;
};


#endif //MENU_H

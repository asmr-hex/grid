#ifndef TEST_FIXTURES_ACTIONS_H
#define TEST_FIXTURES_ACTIONS_H

#include <string>
#include <variant>


namespace fixture {
  namespace action {

    struct increment {};
    struct append { int item; };
    struct clear {};
    struct toggle {};
    struct lowercase {};
    struct change_str{ std::string new_str; };

  }

  using action_t = std::variant<action::increment,
                                action::append,
                                action::clear,
                                action::toggle,
                                action::lowercase,
                                action::change_str>;
}

#endif

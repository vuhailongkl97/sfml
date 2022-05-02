#ifndef _HOME_LONG_WORKING_SFML_HELLO_WORLD_INC_OBSERVER_HPP
#define _HOME_LONG_WORKING_SFML_HELLO_WORLD_INC_OBSERVER_HPP
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

class Subject;
class Observer {
  public:
    virtual auto getID() -> std::string = 0;
    virtual auto update(std::unique_ptr<sf::Vector2f> &&) -> bool = 0;
    virtual ~Observer() = default;
    Observer() = default;
    Observer(const Observer &o) = delete;
    Observer(Observer &&o) = delete;
    auto operator=(const Observer &o) -> Observer & = delete;
    auto operator=(Observer &&o) -> Observer && = delete;
};

class Subject {
  public:
    Subject() = default;
    Subject(const Subject &o) = default;
    Subject(Subject &&o) = delete;
    auto operator=(const Subject &o) -> Subject & = delete;
    auto operator=(Subject &&o) -> Subject && = delete;
    virtual auto attach(std::weak_ptr<Observer> o) -> bool;
    virtual auto detach(std::weak_ptr<Observer> o) -> bool;
    virtual auto get_private_data() -> std::unique_ptr<sf::Vector2f> = 0;
    virtual auto size() -> size_t { return follower.size(); }

    virtual void notify();
    virtual ~Subject() = default;

  private:
    std::vector<std::weak_ptr<Observer>> follower;
};

#endif // _HOME_LONG_WORKING_SFML_HELLO_WORLD_INC_OBSERVER_HPP

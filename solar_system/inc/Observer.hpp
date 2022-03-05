#ifndef _HOME_LONG_WORKING_SFML_HELLO_WORLD_INC_OBSERVER_HPP
#define _HOME_LONG_WORKING_SFML_HELLO_WORLD_INC_OBSERVER_HPP
#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

class Subject;
class Observer {
  public:
    virtual auto getID() -> std::string = 0;
    virtual auto update(std::shared_ptr<void>) -> bool = 0;
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
    virtual auto attach(Observer *o) -> bool;
    virtual auto detach(Observer *o) -> bool;
    virtual auto get_private_data() -> std::shared_ptr<void> = 0;
    virtual auto size() -> size_t { return follower.size(); }

    virtual void notify();
    virtual ~Subject() = default;

  private:
    std::vector<Observer *> follower;
};

#endif // _HOME_LONG_WORKING_SFML_HELLO_WORLD_INC_OBSERVER_HPP

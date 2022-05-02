#ifndef _HOME_LONG_WORKING_SFML_HELLO_WORLD_INC_OBSERVER_HPP
#define _HOME_LONG_WORKING_SFML_HELLO_WORLD_INC_OBSERVER_HPP
#include <iostream>
#include <memory>

class Subject;
class Observer {
  public:
    virtual auto getID() -> std::string = 0;
    virtual auto update(std::shared_ptr<void> data) -> bool = 0;
    virtual ~Observer();
    Observer();
    Observer(const Observer &o) = delete;
    Observer(Observer &&o) = delete;
    auto operator=(const Observer &o) -> Observer & = delete;
    auto operator=(Observer &&o) -> Observer && = delete;
};

class Subject {
  public:
    Subject();
    Subject(const Subject &o) = delete;
    Subject(Subject &&o) = delete;
    virtual ~Subject();

    auto operator=(const Subject &o) -> Subject & = delete;
    auto operator=(Subject &&o) -> Subject && = delete;
    virtual auto attach(std::weak_ptr<Observer> o) -> bool;
    virtual auto detach(std::weak_ptr<Observer> o) -> bool;
    virtual auto size() -> size_t;
    virtual void notify();

  protected:
    virtual auto get_private_data() -> std::shared_ptr<void> = 0;

  private:
    struct Pimpl;
    std::unique_ptr<Pimpl> follower;
};

#endif // _HOME_LONG_WORKING_SFML_HELLO_WORLD_INC_OBSERVER_HPP

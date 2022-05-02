#ifndef ELEMENT_H_
#define ELEMENT_H_

#include "Observer.hpp"
#include <SFML/Graphics.hpp>
#include <memory>

class Star;
class Subject;

class Element : public Subject, public Observer {
  public:
    Element() = default;
    Element(Element &&) = delete;

    Element(const Element &o) : Subject(o) {}

    auto operator=(const Element &o) -> Element & = delete;
    auto operator=(Element &&o) -> Element && = delete;

    auto getID() -> std::string override = 0;
    auto update(std::unique_ptr<sf::Vector2f> &&d) -> bool override = 0;
    auto get_private_data() -> std::unique_ptr<sf::Vector2f> override = 0;
    virtual void setG(const sf::Vector2f &) = 0;
    virtual auto follow(std::weak_ptr<Subject> o) -> bool = 0;
    virtual auto getShape() -> sf::Shape * = 0;
    virtual auto go() -> bool = 0;
    ~Element() override = default;
};

class DecoratorElement : public Element {
  public:
    auto size() -> size_t override { return _elem->size(); }
    void notify() override { _elem->notify(); }
    auto attach(std::weak_ptr<Observer> o) -> bool override {
        return _elem->attach(o);
    }

    auto detach(std::weak_ptr<Observer> o) -> bool override {
        return _elem->detach(o);
    }

    auto getID() -> std::string override { return _elem->getID(); }

    auto update(std::unique_ptr<sf::Vector2f>&& _data) -> bool override {
        return _elem->update(std::move(_data));
    }

    explicit DecoratorElement(std::shared_ptr<Element> elem) : _elem(std::move(elem)) {}

    void setG(const sf::Vector2f &v) override { _elem->setG(v); }

    auto follow(std::weak_ptr<Subject> o) -> bool override {
        return _elem->follow(o.lock());
    }

    auto getShape() -> sf::Shape * override { return _elem->getShape(); }

    auto get_private_data() -> std::unique_ptr<sf::Vector2f> override {
        return _elem->get_private_data();
    }
    auto go() -> bool override { return _elem->go(); }
    DecoratorElement(const DecoratorElement &o) = delete;
    DecoratorElement(DecoratorElement &&o) = delete;
    auto operator=(const DecoratorElement &) -> DecoratorElement = delete;
    auto operator=(DecoratorElement &&) -> DecoratorElement && = delete;

    ~DecoratorElement() override = default;

  protected:
    std::shared_ptr<Element> _elem;
};

#endif // ELEMENT_H_

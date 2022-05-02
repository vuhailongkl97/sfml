#ifndef ELEMENT_H_
#define ELEMENT_H_

#include "Observer.hpp"
#include <SFML/Graphics.hpp>
#include <memory>

class Star;
class Subject;

class Element : public Subject, public Observer {
  public:
    Element();
    Element(Element &&) = delete;
    Element(const Element &o) = delete;
    auto operator=(const Element &o) -> Element & = delete;
    auto operator=(Element &&o) -> Element && = delete;
    auto getID() -> std::string override = 0;
    auto update(std::shared_ptr<void> data) -> bool override = 0;
    auto get_private_data() -> std::shared_ptr<void> override = 0;
    virtual void setG(const sf::Vector2f &) = 0;
    virtual auto follow(std::weak_ptr<Subject> o) -> bool = 0;
    virtual auto getShape() -> sf::Shape * = 0;
    virtual auto go() -> bool = 0;
    ~Element() override;
};

class DecoratorElement : public Element {
  public:
    auto size() -> size_t override;
    void notify() override;
    auto attach(std::weak_ptr<Observer> o) -> bool override;
    auto detach(std::weak_ptr<Observer> o) -> bool override;
    auto getID() -> std::string override;
    auto update(std::shared_ptr<void> data) -> bool override;
    explicit DecoratorElement(std::shared_ptr<Element> elem);
    void setG(const sf::Vector2f &v) override;
    auto follow(std::weak_ptr<Subject> o) -> bool override;
    auto getShape() -> sf::Shape * override;
    auto get_private_data() -> std::shared_ptr<void> override;
    auto go() -> bool override;
    DecoratorElement(const DecoratorElement &o) = delete;
    DecoratorElement(DecoratorElement &&o) = delete;
    auto operator=(const DecoratorElement &) -> DecoratorElement = delete;
    auto operator=(DecoratorElement &&) -> DecoratorElement && = delete;

    ~DecoratorElement() override;

  protected:
    std::shared_ptr<Element> _elem;
};

#endif // ELEMENT_H_

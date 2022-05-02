#include "Element.hpp"

Element::Element() = default;
Element::~Element() = default;

auto DecoratorElement::size() -> size_t { return _elem->size(); }

void DecoratorElement::notify() { _elem->notify(); }

auto DecoratorElement::attach(std::weak_ptr<Observer> o) -> bool {
    return _elem->attach(o);
}

auto DecoratorElement::detach(std::weak_ptr<Observer> o) -> bool {
    return _elem->detach(o);
}

auto DecoratorElement::getID() -> std::string { return _elem->getID(); }

auto DecoratorElement::update(std::shared_ptr<void> data) -> bool {
    return _elem->update(data);
}

DecoratorElement::DecoratorElement(std::shared_ptr<Element> elem)
    : _elem(std::move(elem)) {}

void DecoratorElement::setG(const sf::Vector2f &v) { _elem->setG(v); }

auto DecoratorElement::follow(std::weak_ptr<Subject> o) -> bool {
    return _elem->follow(o.lock());
}

auto DecoratorElement::getShape() -> sf::Shape * { return _elem->getShape(); }

auto DecoratorElement::get_private_data() -> std::shared_ptr<void> {
    return _elem->get_private_data();
}

auto DecoratorElement::go() -> bool { return _elem->go(); }
DecoratorElement::~DecoratorElement() = default;

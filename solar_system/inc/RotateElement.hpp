#ifndef ROTATE_ELEMENT_H_LEMENT_H_
#define ROTATE_ELEMENT_H_LEMENT_H_

#include "Element.hpp"
#include <memory>
#include <utility>

class RotateElement : public DecoratorElement {
  public:
    explicit RotateElement(std::shared_ptr<Element> elem, float angle)
        : _angle(angle), DecoratorElement(std::move(elem)) {}

    auto getShape() -> sf::Shape * override {
        _elem->getShape()->rotate(_angle);
        return _elem->getShape();
    }

  private:
    float _angle;
};

#endif // ROTATE_ELEMENT_H_

#include "Star.hpp"

auto Star::get_private_data() -> std::shared_ptr<void> {
    auto pos = getPosition();
    return std::make_shared<sf::Vector2f>(pos.x, pos.y);
}

auto Star::update(std::shared_ptr<void> data) -> bool {
    std::shared_ptr<sf::Vector2f> _data =
        std::static_pointer_cast<sf::Vector2f>(data);

    this->setG(*_data);
    return true;
}

auto Star::getID() -> std::string { return name; }

auto Star::getShape() -> sf::Shape * { return shape.get(); }

Star::Star(Orbit *ob, sf::Shape *sh, std::string _name)
    : name(std::move(_name)), orbit(ob), shape(sh) {}

Star::Star(const Star &o) : Element(o) {
    this->name = o.name;
    this->orbit = o.orbit;
    this->shape = o.shape;
}

auto Star::go() -> bool {

    auto curPos = this->getPosition();
    auto nstep = this->orbit->get_next();
    shape->setPosition(nstep);
    if (std::sqrt((curPos.x - nstep.x) * (curPos.x - nstep.x) +
                  (curPos.y - nstep.y) * (curPos.y - nstep.y)) >
        OFFSET_LIMIT_FOR_NOTIFY) {
        notify();
    }

    return true;
}

void Star::setG(const sf::Vector2f &pos) { orbit->new_orbit({pos.x, pos.y}); }

auto Star::getPosition() -> sf::Vector2f { return shape->getPosition(); }

auto Star::follow(Subject *o) -> bool { return o->attach(this); }

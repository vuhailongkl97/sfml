#include "Star.hpp"

auto Star::get_private_data() -> std::shared_ptr<void> {
    auto pos = getPosition();
    return std::make_shared<sf::Vector2f>(pos.x, pos.y);
}

auto Star::update(std::shared_ptr<void> data) -> bool {

#ifdef DEBUG
    std::cout << "get update : " << getID() << "\n";
#endif

    auto _data = std::static_pointer_cast<sf::Vector2f>(data);
    this->setG(*_data);
    return true;
}

auto Star::getID() -> std::string { return name; }

auto Star::getShape() -> sf::Shape * { return shape.get(); }

Star::Star(std::unique_ptr<Orbit> &&ob, std::unique_ptr<sf::Shape> &&sh,
           std::string _name)
    : name(std::move(_name)), orbit(std::move(ob)), shape(std::move(sh)) {}

// Star::Star(const Star &o) : Element(o), enable_shared_from_this(o);

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

void Star::setG(const sf::Vector2f &pos) {
    orbit->fillRequiredData(static_cast<const void *>(&pos));
    orbit->fillOrbit();
}

auto Star::getPosition() -> sf::Vector2f { return shape->getPosition(); }

auto Star::follow(std::weak_ptr<Subject> o) -> bool {
    auto spf = o.lock();
    if (spf) {
        //    auto v = shared_from_this();
        return spf->attach((std::shared_ptr<Star>(shared_from_this())));
        // return spf->attach((std::shared_ptr<Star>(this)));
    }
    return false;
}

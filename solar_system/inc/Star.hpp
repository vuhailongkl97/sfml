#ifndef _HOME_LONG_WORKING_SFML_HELLO_WORLD_INC_STAR_HPP
#define _HOME_LONG_WORKING_SFML_HELLO_WORLD_INC_STAR_HPP
#include "Observer.hpp"
#include "Orbit.hpp"

class Star : public Observer, public Subject {

  public:
    ~Star() override = default;

    Star() = default;
    Star(Star &&o) = delete;
    auto operator=(const Star &o) -> Star & = delete;
    auto operator=(Star &&o) -> Star && = delete;
    auto get_private_data() -> std::shared_ptr<void> override;
    auto update(std::shared_ptr<void> data) -> bool override;

    auto getID() -> std::string override;
    void setG(const sf::Vector2f &pos);
    auto getPosition() -> sf::Vector2f;
    auto follow(Star *o) -> bool;
    auto getShape() -> const sf::Shape *;
    Star(Orbit *ob, sf::Shape *sh, std::string _name);
    Star(const Star &o);

    auto go() -> bool;

  private:
    std::string name;
    std::shared_ptr<Orbit> orbit;
    std::shared_ptr<sf::Shape> shape;
};

#endif // _HOME_LONG_WORKING_SFML_HELLO_WORLD_INC_STAR_HPP

#ifndef _HOME_LONG_WORKING_SFML_HELLO_WORLD_INC_STAR_HPP
#define _HOME_LONG_WORKING_SFML_HELLO_WORLD_INC_STAR_HPP
#include "Element.hpp"
#include "Observer.hpp"
#include "Orbit.hpp"

constexpr int OFFSET_LIMIT_FOR_NOTIFY = 7;

class Star : public Element {

  public:
    ~Star() override = default;

    Star() = default;
    Star(Star &&o) = delete;
    auto operator=(const Star &o) -> Star & = delete;
    auto operator=(Star &&o) -> Star && = delete;
    auto get_private_data() -> std::shared_ptr<void> override;
    auto update(std::shared_ptr<void> data) -> bool override;

    auto getID() -> std::string override;
    void setG(const sf::Vector2f &pos) override;
    auto getPosition() -> sf::Vector2f;
    auto follow(Subject *o) -> bool override;
    auto getShape() -> sf::Shape * override;
    Star(Orbit *ob, sf::Shape *sh, std::string _name);
    Star(const Star &o);

    auto go() -> bool override;

  private:
    std::string name;
    std::shared_ptr<Orbit> orbit;
    std::shared_ptr<sf::Shape> shape;
};

#endif // _HOME_LONG_WORKING_SFML_HELLO_WORLD_INC_STAR_HPP

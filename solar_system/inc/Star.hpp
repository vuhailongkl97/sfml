#ifndef _HOME_LONG_WORKING_SFML_HELLO_WORLD_INC_STAR_HPP
#define _HOME_LONG_WORKING_SFML_HELLO_WORLD_INC_STAR_HPP
#include "Element.hpp"
#include "Observer.hpp"
#include "Orbit.hpp"

extern const float OFFSET_LIMIT_FOR_NOTIFY;

class Star : public Element, public std::enable_shared_from_this<Star> {

  public:
    ~Star() override;

    Star() = delete;
    Star(Star &&o) = delete;
    Star(const Star &o) = delete;
    auto operator=(const Star &o) -> Star & = delete;
    auto operator=(Star &&o) -> Star && = delete;
    auto get_private_data() -> std::shared_ptr<void> override;
    auto update(std::shared_ptr<void> data) -> bool override;

    auto getID() -> std::string override;
    void setG(const sf::Vector2f &pos) override;
    auto getPosition() -> sf::Vector2f;
    auto follow(std::weak_ptr<Subject> o) -> bool override;
    auto getShape() -> sf::Shape * override;
    Star(std::unique_ptr<Orbit> &&ob, std::unique_ptr<sf::Shape> &&sh,
         std::string _name);

    auto go() -> bool override;

  private:
    std::string name;
    std::unique_ptr<Orbit> orbit;
    std::unique_ptr<sf::Shape> shape;
};

#endif // _HOME_LONG_WORKING_SFML_HELLO_WORLD_INC_STAR_HPP

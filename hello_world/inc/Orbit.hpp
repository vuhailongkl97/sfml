#ifndef _HOME_LONG_WORKING_SFML_HELLO_WORLD_INC_ORBIT_HPP
#define _HOME_LONG_WORKING_SFML_HELLO_WORLD_INC_ORBIT_HPP

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <vector>

constexpr int THREAD_HOLD = 6;
constexpr int THREAD_HOLD_MAX = 8;
constexpr float REDUCE_SPEED = 0.6;
constexpr float REDUCE_SPEED_MAX = 0.9;

class Orbit {

  public:
    Orbit(sf::Vector2f _I, uint16_t _R, uint8_t speed);

    Orbit(const Orbit &o);

    Orbit(Orbit &&o) = delete;
    ~Orbit() = default;
    auto operator=(const Orbit &o) -> Orbit & = delete;
    auto operator=(Orbit &&o) -> Orbit && = delete;

    void show();
    auto new_orbit(sf::Vector2f _I) -> decltype(true);
    auto setSpeed(uint8_t speed) -> decltype(true);

    auto get_next() -> sf::Vector2f;

  private:
    sf::Vector2f I;
    float R;
    std::vector<sf::Vector2f> Points;
    size_t idx;
    float speed;
};

#endif // _HOME_LONG_WORKING_SFML_HELLO_WORLD_INC_ORBIT_HPP

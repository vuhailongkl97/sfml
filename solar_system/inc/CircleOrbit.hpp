#ifndef CIRCLEORBIT_HPP
#define CIRCLEORBIT_HPP

#include "Orbit.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <vector>

class CircleOrbit : public Orbit {
  public:
    CircleOrbit(sf::Vector2f _I, uint16_t _R, uint8_t speed);
    auto fillRequiredData(const void *_I) -> bool override;
    auto fillOrbit() -> bool override;

  private:
    float R;
    sf::Vector2f I;
};

#endif // CIRCLEORBIT_HPP

#ifndef CIRCLEORBIT_HPP
#define CIRCLEORBIT_HPP

#include "Orbit.hpp"

class CircleOrbit : public Orbit {
  public:
    CircleOrbit(sf::Vector2f _I, uint16_t _R, SPEED speed);
    auto fillRequiredData(const void *_I) -> bool override;
    auto fillOrbit() -> bool override;

  private:
    float R;
    sf::Vector2f I;
};

#endif // CIRCLEORBIT_HPP

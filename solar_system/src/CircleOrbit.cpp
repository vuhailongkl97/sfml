#include "CircleOrbit.hpp"

CircleOrbit::CircleOrbit(sf::Vector2f _I, uint16_t _R, uint8_t speed)
    : Orbit(speed) {
    this->R = _R;

    if (!CircleOrbit::fillRequiredData(static_cast<void *>(&_I))) {
        std::cout << "error when setCenter\n";
        exit(-1);
    }
}

auto CircleOrbit::fillRequiredData(const void *_I) -> bool {
    this->I = *reinterpret_cast<const sf::Vector2f *>(_I);
    return CircleOrbit::fillOrbit();
}

auto CircleOrbit::fillOrbit() -> bool {
    Points.clear();
    float y_prev = 0;
    float x = I.x - R;
    float y = 0;
    y = (I.y + std::sqrt(R * R - (I.x - x) * (I.x - x)));

    y_prev = y;
    Points.emplace_back(x, y);

    while (x < (I.x + R)) {

        auto cb = [this](float x_current) -> float {
            return (this->I.y +
                    std::sqrt(this->R * this->R - (this->I.x - x_current) *
                                                      (this->I.x - x_current)));
        };

        auto ret = calculate_for_inbound(cb, x, y_prev, getSpeed(), I.x - R,
                                         I.x + R, THREAD_HOLD);

        x = ret.x;
        y = ret.y;

        Points.emplace_back(x, y);
        y_prev = y;
    }

    x = I.x + R;
    while (x > I.x - R) {

        auto cb = [this](float x_current) -> float {
            return (this->I.y -
                    std::sqrt(this->R * this->R - (this->I.x - x_current) *
                                                      (this->I.x - x_current)));
        };

        auto ret = calculate_for_inbound(cb, x, y_prev, -1 * getSpeed(),
                                         I.x - R, I.x + R, THREAD_HOLD);
        x = ret.x;
        y = ret.y;

        Points.emplace_back(x, y);
        y_prev = y;
    }
    return !Points.empty();
}

#include "Orbit.hpp"

Orbit::Orbit(sf::Vector2f _I, uint16_t _R, uint8_t speed)
    : I(_I), R(_R), idx(0) {
    this->speed = speed;

    if (!new_orbit(_I)) {
        std::cerr << "new orbit is failed, size = 0\n";
    }
}

Orbit::Orbit(const Orbit &o) {
    this->idx = o.idx;
    this->speed = o.speed;
    this->I = o.I;
    this->R = o.R;
    this->Points = o.Points;
}

void Orbit::show() {
    for (auto &it : Points) {
        std::cout << it.x << ", " << it.y << '\n';
    }
}

constexpr const int ONEHUNDERED = 100;
template <typename T>
auto calculate_for_inbound(T cb, float x_current, float y_prev, float speed,
                           float x_min, float x_max, float thread_hold)
    -> sf::Vector2f {

    float y_test_value = 0;
    float x_for_return = x_current;
    float current_ratio = 1;
    float y_max = 0;
    float y_min = 0;

    do {
        x_for_return = x_current + speed / current_ratio;

        y_test_value = cb(x_for_return);
        y_max = cb(x_max);
        y_min = cb(x_min);

        if (x_for_return > x_max) {
            if (std::fabs(y_max - y_prev) > thread_hold) {
                y_test_value = y_prev + thread_hold + ONEHUNDERED;
                current_ratio *= 2;
                continue;
            }

            x_for_return = x_max;
            y_test_value = y_max;
            break;
        }

        if (x_for_return < x_min) {
            if (std::fabs(y_min - y_prev) > thread_hold) {
                y_test_value = y_prev + thread_hold + ONEHUNDERED;
                current_ratio *= 2;
                continue;
            }

            x_for_return = x_min;
            y_test_value = y_min;
            break;
        }
        current_ratio *= 2;

    } while (std::fabs(y_test_value - y_prev) > thread_hold);

    return {x_for_return, y_test_value};
}

auto Orbit::new_orbit(sf::Vector2f _I) -> decltype(true) {
    Points.clear();
    this->I = _I;
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

        auto ret = calculate_for_inbound(cb, x, y_prev, this->speed, I.x - R,
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

        auto ret = calculate_for_inbound(cb, x, y_prev, -1 * this->speed,
                                         I.x - R, I.x + R, THREAD_HOLD);
        x = ret.x;
        y = ret.y;

        Points.emplace_back(x, y);
        y_prev = y;
    }
    return !Points.empty();
}
auto Orbit::setSpeed(uint8_t speed) -> decltype(true) {
    this->speed = speed;
    return true;
}

auto Orbit::get_next() -> sf::Vector2f {
    if (Points.empty()) {
        std::cerr << "size = 0\n";
        return {0, 0};
    }
    idx++;
    if (idx >= Points.size()) {
        idx = 0;
    }
    return Points.at(idx);
}

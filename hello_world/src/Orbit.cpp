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
auto Orbit::new_orbit(sf::Vector2f _I) -> decltype(true) {
    Points.clear();
    this->I = _I;
    float y_prev = 0;
    float x = I.x - R;
    while (x <= (I.x + R)) {
        float y = (I.y + std::sqrt(R * R - (I.x - x) * (I.x - x)));
        if (static_cast<bool>(y_prev)) {
            if (std::fabs(y - y_prev) > THREAD_HOLD_MAX) {
                x -= this->speed * REDUCE_SPEED_MAX;
                y = (I.y + std::sqrt(R * R - (I.x - x) * (I.x - x)));
            } else if (std::fabs(y - y_prev) > THREAD_HOLD) {
                x -= this->speed * REDUCE_SPEED;
                y = (I.y + std::sqrt(R * R - (I.x - x) * (I.x - x)));
            }
        }

        Points.emplace_back(x, y);
        y_prev = y;
        x += this->speed;
    }

    x = I.x + R;
    while (x >= I.x - R) {
        float y = (I.y - std::sqrt(R * R - (I.x - x) * (I.x - x)));

        if (static_cast<bool>(y_prev) && std::fabs(y - y_prev) > THREAD_HOLD) {
            x += this->speed * REDUCE_SPEED;
            if ((x > (I.x + R))) {
                x = I.x + R;
            }

            y = (I.y - std::sqrt(R * R - (I.x - x) * (I.x - x)));
        }
        Points.emplace_back(x, y);
        y_prev = y;

        x -= this->speed;
    }

    if (Points.size() > 1) {
        for (auto it = Points.begin() + 1; it != Points.end(); it++) {
            if (it->x < (it - 1)->x) {
                Points.erase(it - 1);
                break;
            }
        }
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

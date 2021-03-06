#include "Orbit.hpp"

const float THREAD_HOLD = 6;
const float ONEHUNDERED = 100;

Orbit::Orbit(SPEED _speed) : speed(_speed) {
    std::cout << "need fill data/required data\n";
}

Orbit::Orbit(const Orbit &o) {
    this->idx = o.idx;
    this->speed = o.speed;
}

void Orbit::show() {

    if (Points.empty()) {
        std::cout << "Points empty\n";
        return;
    }

    for (auto &it : Points) {
        std::cout << it.x << ", " << it.y << '\n';
    }
}

auto Orbit::setSpeed(SPEED speed) -> SPEED {
    this->speed = speed;
    return this->speed;
}

auto Orbit::getSpeed() const -> SPEED { return this->speed; }

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

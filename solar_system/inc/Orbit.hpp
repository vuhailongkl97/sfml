#ifndef ORBIT_HPP
#define ORBIT_HPP

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <vector>

constexpr int THREAD_HOLD = 6;
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

class Orbit {

  public:
    explicit Orbit(float speed);
    Orbit(const Orbit &o);
    Orbit(Orbit &&o) = delete;
    virtual ~Orbit() = default;

    auto operator=(const Orbit &o) -> Orbit & = delete;
    auto operator=(Orbit &&o) -> Orbit && = delete;

    auto setSpeed(float speed) -> float;
    auto getSpeed() const -> float;
    auto get_next() -> sf::Vector2f;
    virtual auto fillRequiredData(const void *_I) -> bool = 0;
    virtual auto fillOrbit() -> bool = 0;

    void show();

  protected:
    std::vector<sf::Vector2f> Points;

  private:
    size_t idx;
    float speed;
};

#endif // ORBIT_HPP

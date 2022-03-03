#include "Observer.hpp"
#include "Orbit.hpp"
#include "Star.hpp"
#include <SFML/Graphics.hpp>
#include <cassert>
#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

#ifndef IMAGE_DIR
constexpr const char *IMAGE_DIR = "unknow";
#endif

constexpr const char *background_path = IMAGE_DIR "/galaxy_background.jpg";
constexpr std::array<const char *, 9> Solar_surfix_path = {
    "/sun.png",    "/Mercury.jpeg", "/Venus.png",
    "/Earth.jpeg", "/Mars.jpeg",    "/Jupiter.png",
    "/Saturn.jpg", "/Uranus.png",   "/Neptune.jpeg"};

constexpr int DEFAULT_POINTCOUNT = 30;
constexpr float DEFAULT_ANGLE = 3.0;
constexpr float HALF = 0.5F;
constexpr unsigned int FRAME_RATE = 50;

enum RADIUS_SIZE {
    RADIUS_SIZE_MIN = 8,
    RADIUS_SIZE_MEDIUM = 10,
    RADIUS_SIZE_MAX = 20
};
enum RADIUS {
    RADIUS_MIN = 0,
    RADIUS_LV0 = RADIUS_MIN,
    RADIUS_LV1 = 70,
    RADIUS_LV2 = 110,
    RADIUS_LV3 = 150,
    RADIUS_LV4 = 200,
    RADIUS_LV5 = 250,
    RADIUS_LV6 = 300,
    RADIUS_LV7 = 350,
    RADIUS_LV8 = 400,
    RADIUS_LV9 = 450,
    RADIUS_HIGH = 600
};
static const std::array<RADIUS, 10> RADIUS_LV = {
    RADIUS_LV0, RADIUS_LV1, RADIUS_LV2, RADIUS_LV3, RADIUS_LV4,
    RADIUS_LV5, RADIUS_LV6, RADIUS_LV7, RADIUS_LV8, RADIUS_LV9};

enum SPEED {
    SPEED_MIN = 1,
    SPEED_LV1 = SPEED_MIN,
    SPEED_LV2 = 2,
    SPEED_LV3 = 3,
    SPEED_LV4 = 4,
    SPEED_LV5 = 5,
    SPEED_HIGH = 4
};

auto main() -> int {

    std::vector<std::string> Solar_string;
    Solar_string.reserve(Solar_surfix_path.size());

    for (const auto *it : Solar_surfix_path) {
        Solar_string.push_back(std::string(IMAGE_DIR) + std::string(it));
    }

    const sf::Vector2f DEFAULT_POSITION = {200, 200};
    const sf::Vector2f DEFAULT_WINDOW_SIZE = {800, 800};

    sf::RenderWindow window(
        sf::VideoMode(DEFAULT_WINDOW_SIZE.x, DEFAULT_WINDOW_SIZE.y),
        "SFML hello world!", sf::Style::Default);

    sf::FileInputStream afm;
    sf::Texture texture_background;
    sf::Sprite sprite_background;
    sf::Text text;
    sf::Font font;

    constexpr int SOLAR_STARS_NUMBER = 9;
    std::array<sf::Texture, SOLAR_STARS_NUMBER> solar_textures;
    std::map<std::string, sf::CircleShape *> solar_shapes = {
        {"Sun", new sf::CircleShape(RADIUS_SIZE_MEDIUM, DEFAULT_POINTCOUNT)},
        {"Mercury",
         new sf::CircleShape(RADIUS_SIZE_MEDIUM, DEFAULT_POINTCOUNT)},
        {"Venus", new sf::CircleShape(RADIUS_SIZE_MEDIUM, DEFAULT_POINTCOUNT)},
        {"Earth", new sf::CircleShape(RADIUS_SIZE_MEDIUM, DEFAULT_POINTCOUNT)},
        {"Mars", new sf::CircleShape(RADIUS_SIZE_MEDIUM, DEFAULT_POINTCOUNT)},
        {"Jupiter",
         new sf::CircleShape(RADIUS_SIZE_MEDIUM, DEFAULT_POINTCOUNT)},
        {"Saturn", new sf::CircleShape(RADIUS_SIZE_MEDIUM, DEFAULT_POINTCOUNT)},
        {"Uranus", new sf::CircleShape(RADIUS_SIZE_MEDIUM, DEFAULT_POINTCOUNT)},
        {"Neptune",
         new sf::CircleShape(RADIUS_SIZE_MEDIUM, DEFAULT_POINTCOUNT)}};
    // for (int i = 0; i < solar_shapes.size(); i++) {
    //     solar_textures.at(i).loadFromFile(Solar_string.at(i));
    //     solar_shapes.at(i)->setTexture(&solar_textures.at(i), false);
    // }
    int i = 0;
    for (auto &it : solar_shapes) {
        solar_textures.at(i).loadFromFile(Solar_string.at(i));
        it.second->setTexture(&solar_textures.at(i));
        float x_circle = it.second->getLocalBounds().width * HALF;
        float y_circle = it.second->getLocalBounds().height * HALF;
        it.second->setOrigin(x_circle, y_circle);
        i++;
    }

    std::vector<Star> stars{};
    i = 0;
    for (auto &it : solar_shapes) {
        auto *orbit{new Orbit(DEFAULT_POSITION, RADIUS_LV.at(i), SPEED_LV3)};
        stars.emplace_back(orbit, it.second, it.first);
        i++;
    }

    for (size_t i = 1; i < stars.size(); i++) {
        stars.at(i).follow(&stars.at(0));
    }

    text.setFillColor(sf::Color::Red);
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);

    window.setFramerateLimit(FRAME_RATE);

    try {
        font.loadFromFile("../font/example_font.ttf");
        text.setFont(font);
        text.setString("test");

        texture_background.loadFromFile(background_path, sf::IntRect());
        texture_background.setSmooth(true);
        sprite_background.setTexture(texture_background, false);

    } catch (std::exception &e) {
        std::cout << "error :" << e.what() << "\n";
    }

    // window.setMouseCursorVisible(false);

    sf::String buffer("abc", std::locale());
    float angle = DEFAULT_ANGLE;

    window.setTitle(std::to_string(static_cast<int>(window.hasFocus())));
    window.setVerticalSyncEnabled(true);
    sf::Vector2i localPosition = sf::Mouse::getPosition(window);

    while (window.isOpen()) {
        /* game loop stages
         * 1. Handle input - handle events from input devices and the window
         * 2. Update frame - update objects in the scene
         * 3. Render frame - render objects from the scene onto the window
         * */
        sf::Event event = {.type = sf::Event::Closed};
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::MouseMoved:
                localPosition = sf::Mouse::getPosition(window);
                std::cout << localPosition.x << ", " << localPosition.y << '\n';

                stars.at(0).setG({static_cast<float>(localPosition.x),
                                  static_cast<float>(localPosition.y)});

                break;
            case sf::Event::MouseWheelScrolled:
                if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                    std::cout << "wheel type: vertical" << std::endl;
                } else if (event.mouseWheelScroll.wheel ==
                           sf::Mouse::HorizontalWheel) {
                    std::cout << "wheel type: horizontal" << std::endl;
                } else {
                    std::cout << "wheel type: unknown" << std::endl;
                }
                std::cout << "wheel movement: " << event.mouseWheelScroll.delta
                          << std::endl;
                std::cout << "mouse x: " << event.mouseWheelScroll.x
                          << std::endl;
                std::cout << "mouse y: " << event.mouseWheelScroll.y
                          << std::endl;
                break;
            case sf::Event::MouseEntered:
                std::cout << "the mouse cursor has entered the window"
                          << std::endl;
                break;

            case sf::Event::MouseLeft:
                std::cout << "the mouse cursor has left the window"
                          << std::endl;
                break;
            case sf::Event::EventType::LostFocus: {
                angle *= -1;
                window.setTitle({"lost forcus", std::locale()});
                break;
            }
            case sf::Event::EventType::GainedFocus: {
                angle *= -1;
                window.setTitle({"come backed", std::locale()});
                break;
            }
            case sf::Event::EventType::Closed: {
                window.close();
                break;
            }

            case sf::Event::EventType::TextEntered: {
                buffer += event.text.unicode;
                //    std::cout << buffer.toAnsiString() << "\n";
                break;
            }

            case sf::Event::EventType::KeyPressed:
                if (event.key.code == sf::Keyboard::Key::Space) {
                    window.setTitle({"space was pressed", std::locale()});
                }
                break;
            case sf::Event::EventType::KeyReleased:
                if (event.key.code == sf::Keyboard::Key::Return) {
                    window.setTitle(buffer);
                    buffer.clear();
                } else if (event.key.code == sf::Keyboard::Key::Escape) {
                    window.close();
                }
                break;

            default:
                break;
            }
        }
        // clear window
        window.clear(sf::Color::Black);
        // update frame with draw somethings

        window.draw(sprite_background);
        window.draw(text);
        for (auto &it : stars) {
            it.go();
            if (it.getShape() != nullptr) {
                window.draw(*it.getShape());
            }
        }

        window.display();
    }

    return 0;
}

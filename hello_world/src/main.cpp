#include "Observer.hpp"
#include "Orbit.hpp"
#include "Star.hpp"
#include <SFML/Graphics.hpp>
#include <cassert>
#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

constexpr const char *circle_file_path =
    "/home/long/working/sfml/image/firefox.png";

constexpr int DEFAULT_RADIUS = 10;
constexpr int DEFAULT_POINTCOUNT = 30;
constexpr float DEFAULT_ANGLE = 3.0;
constexpr float HALF = 0.5F;
constexpr unsigned int FRAME_RATE = 60;

enum RADIUS { RADIUS_MIN = 50, RADIUS_MEDIUM = 100, RADIUS_HIGH = 200 };

enum SPEED { SPEED_MIN = 1, SPEED_MEDIUM = 2, SPEED_HIGH = 4 };

auto main() -> int {

    const sf::Vector2f DEFAULT_POSITION = {200, 200};
    const sf::Vector2f DEFAULT_WINDOW_SIZE = {800, 800};
    sf::RenderWindow window(
        sf::VideoMode(DEFAULT_WINDOW_SIZE.x, DEFAULT_WINDOW_SIZE.y),
        "SFML hello world!", sf::Style::Default);
    sf::FileInputStream afm;
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Text text;
    sf::Font font;

    std::vector<std::string> solar_string{circle_file_path, circle_file_path,
                                          circle_file_path};
    std::array<sf::Texture, 3> solar_textures;
    std::array<sf::CircleShape *, 3> solar_shapes = {
        new sf::CircleShape(DEFAULT_RADIUS, DEFAULT_POINTCOUNT),
        new sf::CircleShape(DEFAULT_RADIUS, DEFAULT_POINTCOUNT),
        new sf::CircleShape(DEFAULT_RADIUS, DEFAULT_POINTCOUNT)};
    for (int i = 0; i < 3; i++) {
        solar_textures.at(i).loadFromFile(solar_string.at(i), sf::IntRect());
        solar_shapes.at(i)->setTexture(&solar_textures.at(i), false);
    }

    std::vector<Star> stars{
        {new Orbit(DEFAULT_POSITION, RADIUS_MEDIUM, SPEED_MIN), solar_shapes[0],
         "abc1"},
        {new Orbit(DEFAULT_POSITION, RADIUS_MIN, SPEED_MEDIUM), solar_shapes[1],
         "abc2"},
        {new Orbit(DEFAULT_POSITION, RADIUS_HIGH, SPEED_MIN), solar_shapes[2],
         "abc3"}};

    stars.at(1).follow(&stars.at(0));

    text.setFillColor(sf::Color::Red);
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);

    sprite.setPosition(window.getSize().x * HALF, window.getSize().y * HALF);
    window.setFramerateLimit(FRAME_RATE);
    texture.setRepeated(true);
    try {
        font.loadFromFile("../font/example_font.ttf");
        text.setFont(font);
        text.setString("test");

        texture.loadFromFile(circle_file_path, sf::IntRect());
        texture.setSmooth(true);
        sprite.setTexture(texture, false);
        sprite.setColor(sf::Color::Green);
        sprite.setOrigin(sprite.getLocalBounds().width / 2,
                         sprite.getLocalBounds().height / 2);

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
                for (auto &it : stars) {
                    it.setG({static_cast<float>(localPosition.x),
                             static_cast<float>(localPosition.y)});
                }
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
        window.clear(sf::Color::Blue);
        // update frame with draw somethings

        // render frame
        // sprite.rotate(angle);
        // window.draw(sprite);

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

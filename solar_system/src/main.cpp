#include "CircleOrbit.hpp"
#include "Observer.hpp"
#include "RotateElement.hpp"
#include "Star.hpp"
#include <SFML/Graphics.hpp>
#include <cassert>
#include <cmath>
#include <iostream>
#include <map>
#include <memory>
#include <vector>

#ifndef IMAGE_DIR
constexpr const char *IMAGE_DIR = "unknow";
#endif

constexpr const char *background_path = IMAGE_DIR "/galaxy_background.jpg";
constexpr int SOLAR_STARS_NUMBER = 10;
constexpr std::array<const char *, SOLAR_STARS_NUMBER> Solar_surfix_path = {
    "/sun.png",      "/Mercury.jpeg", "/Venus.png",  "/Earth.jpeg",
    "/Mars.jpeg",    "/Jupiter.png",  "/Saturn.jpg", "/Uranus.png",
    "/Neptune.jpeg", "/Neptune.jpeg"};

constexpr int DEFAULT_POINTCOUNT = 30;
constexpr float DEFAULT_ANGLE = 3.0;
constexpr float HALF = 0.5F;
constexpr unsigned int FRAME_RATE = 50;

enum RADIUS_SIZE {
    RADIUS_SIZE_MIN = 3,
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
    RADIUS_MOON = 20,
    RADIUS_HIGH = 600
};

static const std::array<RADIUS, 10> RADIUS_LV = {
    RADIUS_LV0, RADIUS_LV1, RADIUS_LV2, RADIUS_LV3, RADIUS_LV4,
    RADIUS_LV5, RADIUS_LV6, RADIUS_LV7, RADIUS_LV8, RADIUS_MOON};

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

    std::vector<std::string> Solar_string{};
    const sf::Vector2f DEFAULT_POSITION = {200, 200};
    const sf::Vector2f DEFAULT_WINDOW_SIZE = {800, 800};

    sf::FileInputStream afm;
    sf::Texture texture_background;
    sf::Sprite sprite_background;
    sf::Text text;
    sf::Font font;

    sf::RenderWindow window(
        sf::VideoMode(DEFAULT_WINDOW_SIZE.x, DEFAULT_WINDOW_SIZE.y),
        "SFML hello world!", sf::Style::Default);

    Solar_string.reserve(Solar_surfix_path.size());

    // add prefix for image
    for (const auto *it : Solar_surfix_path) {
        Solar_string.push_back(std::string(IMAGE_DIR) + std::string(it));
    }

    window.setMouseCursorVisible(false);
    window.setFramerateLimit(FRAME_RATE);
    window.setTitle(std::to_string(static_cast<int>(window.hasFocus())));
    window.setVerticalSyncEnabled(true);

    std::array<sf::Texture, SOLAR_STARS_NUMBER> solar_textures;
    std::map<std::string, sf::CircleShape *> solar_shapes = {
        {"0Sun", new sf::CircleShape(RADIUS_SIZE_MAX, DEFAULT_POINTCOUNT)},
        {"1Mercury",
         new sf::CircleShape(RADIUS_SIZE_MEDIUM, DEFAULT_POINTCOUNT)},
        {"2Venus", new sf::CircleShape(RADIUS_SIZE_MEDIUM, DEFAULT_POINTCOUNT)},
        {"3Earth", new sf::CircleShape(RADIUS_SIZE_MEDIUM, DEFAULT_POINTCOUNT)},
        {"4Mars", new sf::CircleShape(RADIUS_SIZE_MEDIUM, DEFAULT_POINTCOUNT)},
        {"5Jupiter",
         new sf::CircleShape(RADIUS_SIZE_MEDIUM, DEFAULT_POINTCOUNT)},
        {"6Saturn",
         new sf::CircleShape(RADIUS_SIZE_MEDIUM, DEFAULT_POINTCOUNT)},
        {"7Uranus",
         new sf::CircleShape(RADIUS_SIZE_MEDIUM, DEFAULT_POINTCOUNT)},
        {"8Neptune",
         new sf::CircleShape(RADIUS_SIZE_MEDIUM, DEFAULT_POINTCOUNT)},
        {"9Moon", new sf::CircleShape(RADIUS_SIZE_MIN, DEFAULT_POINTCOUNT)}};

    int i = 0;
    for (auto &it : solar_shapes) {
        solar_textures.at(i).loadFromFile(Solar_string.at(i));
        it.second->setTexture(&solar_textures.at(i));
        it.second->setOutlineThickness(0);
        float x_circle = it.second->getLocalBounds().width * HALF;
        float y_circle = it.second->getLocalBounds().height * HALF;
        it.second->setOrigin(x_circle, y_circle);
        i++;
    }

    std::map<std::string, Element *> stars{};
    i = 0;
    try {

        for (auto &it : solar_shapes) {
            auto *orbit{
                new CircleOrbit(DEFAULT_POSITION, RADIUS_LV.at(i), SPEED_LV3)};
            stars.insert(std::pair<std::string, Element *>(
                it.first.c_str(),
                new RotateElement(new Star(orbit, it.second, it.first),
                                  DEFAULT_ANGLE)));
            i++;
        }
    } catch (std::exception &e) {
        std::cout << "errror : " << e.what();
        exit(0);
    }

    stars.insert(std::pair<std::string, Element *>("9Moon", stars["9Moon"]));

    auto *sun_as_subject = dynamic_cast<Subject *>(stars["0Sun"]);

    if (sun_as_subject == nullptr) {
        std::cout << "can't downcast Sun -> Subject*\n";
        exit(0);
    }

    Subject *earth_as_subject = dynamic_cast<Subject *>(stars["3Earth"]);

    if (earth_as_subject == nullptr) {
        std::cout << "can't downcast Earth -> Subject*\n";
        exit(0);
    }

    stars["9Moon"]->follow(earth_as_subject);
    stars["1Mercury"]->follow(sun_as_subject);
    stars["2Venus"]->follow(sun_as_subject);
    stars["3Earth"]->follow(sun_as_subject);
    stars["4Mars"]->follow(sun_as_subject);
    stars["5Jupiter"]->follow(sun_as_subject);
    stars["6Saturn"]->follow(sun_as_subject);
    stars["7Uranus"]->follow(sun_as_subject);
    stars["8Neptune"]->follow(sun_as_subject);

    try {

        texture_background.loadFromFile(background_path, sf::IntRect());
        texture_background.setSmooth(true);
        sprite_background.setTexture(texture_background, false);

    } catch (std::exception &e) {
        std::cout << "error :" << e.what() << "\n";
        exit(0);
    }

    sf::String buffer("abc", std::locale());
    float angle = DEFAULT_ANGLE;

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
                // std::cout << localPosition.x << ", " << localPosition.y <<
                // '\n';

                stars["0Sun"]->setG({static_cast<float>(localPosition.x),
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
            it.second->go();
            if (it.second->getShape() != nullptr) {
                window.draw(*(it.second->getShape()));
            }
        }

        window.display();
    }

    return 0;
}

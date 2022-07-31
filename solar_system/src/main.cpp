#include "CircleOrbit.hpp"
#include "RotateElement.hpp"
#include "Star.hpp"
#include <SFML/Graphics.hpp>
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

enum class RADIUS_SIZE { MIN = 3, MEDIUM = 10, MAX = 20 };
enum class RADIUS : std::uint16_t {
    MIN = 0,
    LV0 = MIN,
    LV1 = 70,
    LV2 = 110,
    LV3 = 150,
    LV4 = 200,
    LV5 = 250,
    LV6 = 300,
    LV7 = 350,
    LV8 = 400,
    LV9 = 450,
    MOON = 20,
    HIGH = 600
};

static const std::array<RADIUS, 10> RADIUS_LV{
    RADIUS::LV0, RADIUS::LV1, RADIUS::LV2, RADIUS::LV3, RADIUS::LV4,
    RADIUS::LV5, RADIUS::LV6, RADIUS::LV7, RADIUS::LV8, RADIUS::MOON};

auto main() -> int {

    std::vector<std::string> Solar_string{};
    const sf::Vector2u DEFAULT_POSITION = {200, 200};
    const sf::Vector2f DEFAULT_POSITIONF = {200, 200};
    const sf::Vector2u DEFAULT_WINDOW_SIZE = {800, 800};

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

    auto gen_map =
        []() -> std::map<std::string, std::unique_ptr<sf::CircleShape>> {
        std::map<std::string, std::unique_ptr<sf::CircleShape>> ret;

        ret["0Sun"] = std::unique_ptr<sf::CircleShape>(new sf::CircleShape(
            static_cast<float>(RADIUS_SIZE::MAX), DEFAULT_POINTCOUNT));
        ret["1Mercury"] = std::unique_ptr<sf::CircleShape>(new sf::CircleShape(
            static_cast<float>(RADIUS_SIZE::MEDIUM), DEFAULT_POINTCOUNT));
        ret["2Venus"] = std::unique_ptr<sf::CircleShape>(new sf::CircleShape(
            static_cast<float>(RADIUS_SIZE::MEDIUM), DEFAULT_POINTCOUNT));
        ret["3Earth"] = std::unique_ptr<sf::CircleShape>(new sf::CircleShape(
            static_cast<float>(RADIUS_SIZE::MEDIUM), DEFAULT_POINTCOUNT));
        ret["4Mars"] = std::unique_ptr<sf::CircleShape>(new sf::CircleShape(
            static_cast<float>(RADIUS_SIZE::MEDIUM), DEFAULT_POINTCOUNT));
        ret["5Jupiter"] = std::unique_ptr<sf::CircleShape>(new sf::CircleShape(
            static_cast<float>(RADIUS_SIZE::MEDIUM), DEFAULT_POINTCOUNT));
        ret["6Saturn"] = std::unique_ptr<sf::CircleShape>(new sf::CircleShape(
            static_cast<float>(RADIUS_SIZE::MEDIUM), DEFAULT_POINTCOUNT));
        ret["7Uranus"] = std::unique_ptr<sf::CircleShape>(new sf::CircleShape(
            static_cast<float>(RADIUS_SIZE::MEDIUM), DEFAULT_POINTCOUNT));
        ret["8Neptune"] = std::unique_ptr<sf::CircleShape>(new sf::CircleShape(
            static_cast<float>(RADIUS_SIZE::MEDIUM), DEFAULT_POINTCOUNT));
        ret["9Moon"] = std::unique_ptr<sf::CircleShape>(new sf::CircleShape(
            static_cast<float>(RADIUS_SIZE::MIN), DEFAULT_POINTCOUNT));
        return ret;
    };
    auto solar_shapes = gen_map();

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

    std::map<std::string, std::shared_ptr<Element>> stars;
    std::weak_ptr<Element> wptr;
    i = 0;
    try {

        for (auto &it : solar_shapes) {
            auto orbit{std::unique_ptr<CircleOrbit>(new CircleOrbit(
                DEFAULT_POSITIONF, static_cast<uint16_t>(RADIUS_LV.at(i)),
                SPEED::LV3))};
            auto shape_ptr = std::unique_ptr<sf::Shape>(
                static_cast<sf::Shape *>(it.second.release()));
            auto v = std::make_shared<RotateElement>(
                std::make_shared<Star>(std::move(orbit), std::move(shape_ptr),
                                       it.first),
                DEFAULT_ANGLE);
            stars.insert(std::pair<std::string, std::shared_ptr<Element>>(
                it.first.c_str(), v));
            i++;
        }
    } catch (std::exception &e) {
        std::cout << "errror : " << e.what();
        exit(0);
    }

    std::shared_ptr<Subject> sun_as_subject =
        std::static_pointer_cast<Subject>(stars["0Sun"]);
    std::shared_ptr<Subject> earth_as_subject =
        std::static_pointer_cast<Subject>(stars["3Earth"]);

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

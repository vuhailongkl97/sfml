#include <SFML/Graphics.hpp>
#include <cassert>
#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

constexpr const char *circle_file_path =
    "/home/long/working/sfml/image/firefox.png";

constexpr int THREAD_HOLD = 6;
constexpr int THREAD_HOLD_MAX = 8;
constexpr float REDUCE_SPEED = 0.6;
constexpr float REDUCE_SPEED_MAX = 0.9;
constexpr int DEFAULT_RADIUS = 10;
constexpr int DEFAULT_POINTCOUNT = 30;
constexpr float DEFAULT_ANGLE = 3.0;
constexpr float HALF = 0.5F;
constexpr unsigned int FRAME_RATE = 60;

enum RADIUS {
    RADIUS_MIN = 50,
    RADIUS_MEDIUM=100,
    RADIUS_HIGH=200
};

enum SPEED {
    SPEED_MIN = 1,
    SPEED_MEDIUM=2,
    SPEED_HIGH=4
};


class Orbit {

  public:
    Orbit(sf::Vector2f _I, uint16_t _R, uint8_t speed) : I(_I), R(_R), idx(0) {
        this->speed = speed;

        if (!new_orbit(_I)) {
            std::cerr << "new orbit is failed, size = 0\n";
        }
    }

    Orbit(const Orbit &o) {
        this->idx = o.idx;
        this->speed = o.speed;
        this->I = o.I;
        this->R = o.R;
        this->Points = o.Points;
    }

    Orbit(Orbit &&o) = delete;
    ~Orbit() = default;
    auto operator=(const Orbit &o) -> Orbit & = delete;
    auto operator=(Orbit &&o) -> Orbit && = delete;

    void show() {
        for (auto &it : Points) {
            std::cout << it.x << ", " << it.y << '\n';
        }
    }
    auto new_orbit(sf::Vector2f _I) -> decltype(true) {
        Points.clear();
        this->I = _I;
        float y_prev = 0;
        float x = I.x - R;
        while (x <= (I.x + R)) {
            float y = (I.y + std::sqrt(R * R - (I.x - x) * (I.x - x)));
            if (static_cast<bool>(y_prev))
            {
                if(std::fabs(y - y_prev) > THREAD_HOLD_MAX)
                {
                    x -= this->speed * REDUCE_SPEED_MAX;
                    y = (I.y + std::sqrt(R * R - (I.x - x) * (I.x - x)));
                }
                else if(std::fabs(y - y_prev) > THREAD_HOLD)
                {
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

            if (static_cast<bool>(y_prev) &&
                std::fabs(y - y_prev) > THREAD_HOLD) {
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

        if(Points.size() > 1)
        {
            for( auto it = Points.begin() + 1; it != Points.end(); it++)
            {
                if(it->x < (it - 1)->x)
                {
                    Points.erase(it -1);
                    break;
                }
            }
        }

        return !Points.empty();
    }
    auto setSpeed(uint8_t speed) -> decltype(true) {
        this->speed = speed;
        return true;
    }

    auto get_next() -> sf::Vector2f {
        if (Points.empty()) {
            std::cerr << "size = 0\n";
            return {0,0};
        }
        idx++;
        if (idx >= Points.size()) {
            idx = 0;
        }
        return Points.at(idx);
    }

  private:
    sf::Vector2f I;
    float R;
    std::vector<sf::Vector2f> Points;
    size_t idx;
    float speed;
};

class Subject;
class Observer {
  public:
    virtual auto getID() -> std::string  = 0;
    virtual auto update(std::shared_ptr<void>) -> bool = 0;
    virtual ~Observer() = default;
    Observer() = default;
    Observer(const Observer&o) = delete;
    Observer(Observer&&o) = delete;
    auto operator=(const Observer&o) -> Observer& = delete;
    auto operator=(Observer&&o) -> Observer&& = delete;
};

class Subject {
  public:

    Subject() = default;
    Subject(const Subject&o) = default;
    Subject(Subject&&o) = delete;
    auto operator=(const Subject&o) -> Subject& = delete;
    auto operator=(Subject&&o) -> Subject&& = delete;
    auto attach(Observer *o) -> bool{
        follower.push_back(o);
        return true;
    }
    auto detach(Observer *o) -> bool{
        auto it = std::find_if(follower.begin(), follower.end(), [o](Observer *v) -> bool {
                return v->getID() == o->getID();
        });
        if(it != follower.end())
        {
            follower.erase(it);
        }

        return true;
    }

    virtual auto get_private_data() -> std::shared_ptr<void> = 0;

    void notify() {
        for (auto &f : follower) {
            f->update(get_private_data());
        }
    }
    virtual ~Subject() = default;

  private:
    std::vector<Observer *> follower;
};

class Star : public Observer, public Subject {

  public:
    ~Star() override = default;

    Star() = default;
    Star(Star&&o) = delete;
    auto operator=(const Star&o) -> Star& = delete;
    auto operator=(Star&&o) -> Star&& = delete;
    auto get_private_data() -> std::shared_ptr<void> override {
        auto pos = getPosition();
        return std::make_shared<sf::Vector2f>(pos.x, pos.y);
    }

    auto update(std::shared_ptr<void> data) -> bool override {
        std::shared_ptr<sf::Vector2f> _data =
            std::static_pointer_cast<sf::Vector2f>(data);
        std::cout << "update with data pos is " << _data->x << ", " << _data->y
                  << "\n";
        this->setG(*_data);
        return true;
    }

    auto getID()  -> std::string override { return name; }
    void setG(const sf::Vector2f &pos);
    auto getPosition() -> sf::Vector2f;
    auto follow(Star *o) -> bool;
    auto  getShape() -> const sf::Shape* { return shape.get(); }
    Star(Orbit *ob, sf::Shape *sh, std::string _name)
        : name(std::move(_name)), orbit(ob), shape(sh) {}
    Star(const Star &o): Subject(o) {
        this->name = o.name;
        this->orbit = o.orbit;
        this->shape = o.shape;
    }

    auto go() -> bool{
        auto nstep = this->orbit->get_next();
        shape->setPosition(nstep);
        notify();

        return true;
    }

  private:
    std::string name;
    std::shared_ptr<Orbit> orbit;
    std::shared_ptr<sf::Shape> shape;
};

void Star::setG(const sf::Vector2f &pos) { orbit->new_orbit({pos.x, pos.y}); }

auto Star::getPosition() -> sf::Vector2f { return shape->getPosition(); }

auto Star::follow(Star *o) -> bool {
    return o->attach(this);
}

auto main() -> int {

    const sf::Vector2f DEFAULT_POSITION = {200,200};
    const sf::Vector2f DEFAULT_WINDOW_SIZE = {800,800};
    sf::RenderWindow window(sf::VideoMode(DEFAULT_WINDOW_SIZE.x, DEFAULT_WINDOW_SIZE.y), "SFML hello world!",
                            sf::Style::Default);
    sf::FileInputStream afm;
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Text text;
    sf::Font font;

    std::vector<std::string> solar_string{circle_file_path, circle_file_path,
                                          circle_file_path};
    std::array<sf::Texture, 3> solar_textures;
    std::array<sf::CircleShape*, 3> solar_shapes = {new sf::CircleShape(DEFAULT_RADIUS, DEFAULT_POINTCOUNT ),
                                        new sf::CircleShape(DEFAULT_RADIUS, DEFAULT_POINTCOUNT ),
                                        new sf::CircleShape(DEFAULT_RADIUS, DEFAULT_POINTCOUNT )};
    for (int i = 0; i < 3; i++) {
        solar_textures.at(i).loadFromFile(solar_string.at(i), sf::IntRect());
        solar_shapes.at(i)->setTexture(&solar_textures.at(i), false);
    }

    std::vector<Star> stars{
        {new Orbit(DEFAULT_POSITION, RADIUS_MEDIUM, SPEED_MIN), solar_shapes[0], "abc1"},
        {new Orbit(DEFAULT_POSITION, RADIUS_MIN, SPEED_MEDIUM), solar_shapes[1], "abc2"},
        {new Orbit(DEFAULT_POSITION, RADIUS_HIGH, SPEED_MIN), solar_shapes[2], "abc3"}};

    stars.at(1).follow(&stars.at(0));

    text.setFillColor(sf::Color::Red);
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);

    sprite.setPosition(window.getSize().x * HALF, window.getSize().y *HALF);
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
        sf::Event event= {.type = sf::Event::Closed};
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::MouseMoved:
                localPosition = sf::Mouse::getPosition(window);
                std::cout << localPosition.x << ", " << localPosition.y << '\n';
                for (auto &it : stars) {
                    it.setG({static_cast<float>(localPosition.x), static_cast<float>(localPosition.y)});
                }
                break;
            case sf::Event::MouseWheelScrolled:
                if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
                {
                    std::cout << "wheel type: vertical" << std::endl;
                }
                else if (event.mouseWheelScroll.wheel ==
                         sf::Mouse::HorizontalWheel)
                {
                    std::cout << "wheel type: horizontal" << std::endl;
                }
                else
                {
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

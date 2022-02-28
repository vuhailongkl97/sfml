#include <SFML/Graphics.hpp>
#include <assert.h>
#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

const std::string circle_file_path =
    "/home/long/working/sfml/image/firefox.png";

class Orbit {

  public:
    sf::Vector2f calculate_next(int x) {
        return sf::Vector2f(x, (sqrt(R * R - (x - I.x) * (x - I.x)) + I.y));
    }

    Orbit(sf::Vector2i _I, uint16_t _R, uint8_t speed) : I(_I), R(_R), idx(0) {
        this->speed = speed;

        if (false == new_orbit(_I)) {
            std::cerr << "new orbit is failed, size = 0\n";
        }
    }

    Orbit(const Orbit &o) {
        this->speed = o.speed;
        this->I = o.I;
        this->R = o.R;
        this->Points = o.Points;
    }

    Orbit(Orbit &&o) = delete;
    Orbit &operator=(const Orbit &o) = delete;
    Orbit &&operator=(Orbit &&o) = delete;

    void show() {
        for (auto &it : Points) {
            std::cout << it.x << ", " << it.y << '\n';
        }
    }

    bool new_orbit(sf::Vector2i _I) {
        Points.clear();
        this->I = _I;
        float y_prev = 0;
        for (float x = I.x - R; x <= I.x + R; x += this->speed) {
            float y = (I.y + sqrt(R * R - (I.x - x) * (I.x - x)));
            if (y_prev && fabs(y - y_prev) > 7) {
                x -= this->speed * 0.60;
                y = (I.y + sqrt(R * R - (I.x - x) * (I.x - x)));
            }

            Points.push_back({x, y});
            y_prev = y;
        }

        for (float x = I.x + R; x >= I.x - R; x -= this->speed) {
            float y = (I.y - sqrt(R * R - (I.x - x) * (I.x - x)));

            if (y_prev && fabs(y - y_prev) > 8) {
                x += this->speed * 0.60;
                if (x > I.x + R)
                    x = I.x + R;

                y = (I.y - sqrt(R * R - (I.x - x) * (I.x - x)));
            }
            Points.push_back({x, y});
            y_prev = y;
        }
        return Points.size() != 0;
    }
    bool setSpeed(uint8_t speed) {
        this->speed = speed;
        return true;
    }

    sf::Vector2f get_next() {
        if (Points.size() == 0) {
            std::cerr << "size = 0\n";
        }
        idx++;
        if (idx >= Points.size()) {
            idx = 0;
        }
        return Points.at(idx);
    }

  private:
    sf::Vector2i I;
    uint16_t R;
    std::vector<sf::Vector2f> Points;
    size_t idx;
    uint8_t speed;
};

class Subject;
class Observer {
  public:
    virtual std::string getID() const = 0;
    virtual bool update(std::shared_ptr<void>) = 0;
    ~Observer() {}
};

class Subject {
  public:
    bool attach(Observer *o) {
        follower.push_back(o);
        return true;
    }
    bool detach(Observer *o) {
        follower.erase(std::remove_if(
            follower.begin(), follower.end(),
            [o](const Observer *v) { return v->getID() == o->getID(); }));
        return true;
    }

    virtual std::shared_ptr<void> get_private_data() = 0;

    void notify() {
        for (auto &f : follower) {
            f->update(get_private_data());
        }
    }
    ~Subject() {}

  private:
    std::vector<Observer *> follower;
};

class Star : public Observer, public Subject {

  public:
    std::shared_ptr<void> get_private_data() {
        auto pos = getPosition();
        return std::make_shared<sf::Vector2f>(pos.x, pos.y);
    }

    bool update(std::shared_ptr<void> data) {
        std::shared_ptr<sf::Vector2f> _data =
            std::static_pointer_cast<sf::Vector2f>(data);
        std::cout << "update with data pos is " << _data->x << ", " << _data->y
                  << "\n";
        this->setG(*_data);
        return true;
    }

    std::string getID() const { return name; }
    void setG(const sf::Vector2f &pos);
    sf::Vector2f getPosition();
    bool follow(Star &o);
    const sf::Shape *getShape() { return shape.get(); }
    Star(Orbit *ob, sf::Shape *sh, std::string _name = "Star_default")
        : name(_name), orbit(ob), shape(sh) {}
    Star(const Star &o) {
        this->name = o.name;
        this->orbit = o.orbit;
        this->shape = o.shape;
    }

    bool go() {
        auto nstep = this->orbit->get_next();
        shape->setPosition({(float)nstep.x, (float)nstep.y});
        notify();

        return true;
    }

  private:
    std::string name;
    std::shared_ptr<Orbit> orbit;
    std::shared_ptr<sf::Shape> shape;
};

void Star::setG(const sf::Vector2f &pos) {
    orbit->new_orbit({(int)pos.x, (int)pos.y});
}

sf::Vector2f Star::getPosition() { return shape->getPosition(); }

bool Star::follow(Star &o) {
    return orbit->new_orbit({(int)o.getPosition().x, (int)o.getPosition().y});
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "SFML hello world!",
                            sf::Style::Default);
    sf::FileInputStream afm;
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Text text;
    sf::Font font;

    std::vector<std::string> solar_string{circle_file_path, circle_file_path,
                                          circle_file_path};
    sf::Texture solar_textures[3];
    sf::CircleShape *solar_shapes[3] = {new sf::CircleShape(10),
                                        new sf::CircleShape(10),
                                        new sf::CircleShape(10)};
    for (int i = 0; i < 3; i++) {
        solar_textures[i].loadFromFile(solar_string.at(i));
        solar_shapes[i]->setTexture(&solar_textures[i]);
    }

    std::vector<Star> stars{
        {new Orbit({200, 200}, 100, 1), solar_shapes[0], "abc1"},
        {new Orbit({200, 200}, 50, 2), solar_shapes[1], "abc2"},
        {new Orbit({200, 200}, 300, 1), solar_shapes[2], "abc3"}};

    stars.at(0).attach(&stars.at(1));

    text.setFillColor(sf::Color::Red);
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);

    sprite.setPosition(window.getSize().x / 2, window.getSize().y / 2);
    window.setFramerateLimit(60);
    texture.setRepeated(true);
    try {
        assert(font.loadFromFile("../font/example_font.ttf"));
        text.setFont(font);
        text.setString("test");

        texture.loadFromFile(circle_file_path);
        texture.setSmooth(true);
        sprite.setTexture(texture);
        sprite.setColor(sf::Color::Green);
        sprite.setOrigin(sprite.getLocalBounds().width / 2,
                         sprite.getLocalBounds().height / 2);

    } catch (std::exception &e) {
        std::cout << "error :" << e.what() << "\n";
    }

    // window.setMouseCursorVisible(false);

    sf::String buffer("abc");
    float angle = 3.0;

    window.setTitle(std::to_string(window.hasFocus()));
    window.setVerticalSyncEnabled(true);
    sf::Vector2i localPosition = sf::Mouse::getPosition(window);

    while (window.isOpen()) {
        /* game loop stages
         * 1. Handle input - handle events from input devices and the window
         * 2. Update frame - update objects in the scene
         * 3. Render frame - render objects from the scene onto the window
         * */
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::MouseMoved: {
                localPosition = sf::Mouse::getPosition(window);
                std::cout << localPosition.x << ", " << localPosition.y << '\n';
                for (auto &it : stars) {
                    it.setG({(float)localPosition.x, (float)localPosition.y});
                }
                break;
            }
            case sf::Event::MouseWheelScrolled: {
                if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
                    std::cout << "wheel type: vertical" << std::endl;
                else if (event.mouseWheelScroll.wheel ==
                         sf::Mouse::HorizontalWheel)
                    std::cout << "wheel type: horizontal" << std::endl;
                else
                    std::cout << "wheel type: unknown" << std::endl;
                std::cout << "wheel movement: " << event.mouseWheelScroll.delta
                          << std::endl;
                std::cout << "mouse x: " << event.mouseWheelScroll.x
                          << std::endl;
                std::cout << "mouse y: " << event.mouseWheelScroll.y
                          << std::endl;
                break;
            }
            case sf::Event::MouseEntered: {
                std::cout << "the mouse cursor has entered the window"
                          << std::endl;
                break;
            }

            case sf::Event::MouseLeft: {
                std::cout << "the mouse cursor has left the window"
                          << std::endl;
                break;
            }
            case sf::Event::EventType::LostFocus: {
                angle *= -1;
                window.setTitle("lost forcus");
                break;
            }
            case sf::Event::EventType::GainedFocus: {
                angle *= -1;
                window.setTitle("come backed");
                break;
            }
            case sf::Event::EventType::Closed: {
                window.close();
                break;
            }

            case sf::Event::EventType::TextEntered: {
                buffer += event.text.unicode;
                std::cout << buffer.toAnsiString() << "\n";
                break;
            }

            case sf::Event::EventType::KeyPressed:
                if (event.key.code == sf::Keyboard::Key::Space) {
                    window.setTitle("space was pressed");
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
            if (it.getShape()) {
                window.draw(*it.getShape());
            }
        }

        window.display();
    }

    return 0;
}

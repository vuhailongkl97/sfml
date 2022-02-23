#include <SFML/Graphics.hpp>
#include <iostream>
#include <assert.h>
#include <vector>
#include <cmath>
#include <memory>

const char * circle_file_path = "/home/long/working/sfml/image/circle.png";

class Orbit {

    public:
    sf::Vector2f calculate_next(int x)
    {
        return sf::Vector2f(x, (sqrt(R*R - (x - I.x)*(x- I.x)) + I.y));
    }


    Orbit(sf::Vector2i _I, uint16_t _R, sf::Shape *sh, uint8_t speed): I(_I), R(_R), shape(sh), idx(0)
    {
        this->speed = speed;
        new_orbit(_I);
    }

    Orbit(const Orbit &o)
    {
        this->speed = o.speed;
        this->I = o.I;
        this->R = o.R;
        sf::CircleShape* ptr = dynamic_cast<sf::CircleShape*>(o.shape.get());
        if(ptr ==nullptr)
        {
            std::cerr<< "error cast\n";
            exit(0);
        }
        this->shape = std::make_unique<sf::CircleShape>(ptr->getRadius());
        this->Points = o.Points;
    }

    Orbit(Orbit &&o) = delete;
    Orbit & operator = (const Orbit &o) = delete;
    Orbit && operator = (Orbit && o) = delete;

    void new_orbit(sf::Vector2i _I)
    {
        Points.clear();
        this->I = _I;
        for(int x = I.x -R ; x < I.x + R; x+=this->speed)
        {
           int y = (2.0*I.y + 2*sqrt(R*R - (I.x -x)*(I.x -x)))/2.0;
           Points.push_back({x,y});
        }

        for(int x = I.x +R ; x >= I.x - R; x-=this->speed)
        {
           int y = (2.0*I.y - 2*sqrt(R*R - (I.x -x)*(I.x -x)))/2.0;
           Points.push_back({x,y});
        }
    }
    bool setSpeed(uint8_t speed){
        this->speed = speed;
        return true;
    }

        sf::Vector2i get_next()
        {
            if(Points.size() == 0)
            {
                std::cerr << "size = 0\n";
            }
            idx ++;
            if(idx >= Points.size())
            {
                idx= 0;
            }
            return Points.at(idx);
        }

    void show()
    {
        for(auto i:Points)
        {
            std::cout << "(" << i.x << ", " << i.y << ")\n";
        }

    }
    const sf::Shape* getShape()
    {
        return shape.get();
    }

    void setPosition4Shape(float x, float y)
    {
        this->shape->setPosition(x,y);
    }

    private:
        sf::Vector2i I;
        uint16_t R;
        std::unique_ptr<sf::Shape> shape;
        std::vector<sf::Vector2i> Points;
        uint16_t idx;
        uint8_t speed;
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(800,800), "SFML hello world!", sf::Style::Default);
    sf::FileInputStream afm;
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Text text;
    sf::Font font;
    std::vector<Orbit> stars{{{200,200}, 100, new sf::CircleShape(10),2},
        {{200,200}, 200, new sf::CircleShape(10), 1},
        {{200,200}, 250, new sf::CircleShape(10), 3},
        {{200,200}, 300, new sf::CircleShape(10), 5}  
    };

    text.setFillColor(sf::Color::Red);
    text.setStyle(sf::Text::Bold| sf::Text::Underlined);

    sprite.setPosition(window.getSize().x/2, window.getSize().y/2);
    window.setFramerateLimit(60);
    texture.setRepeated(true);
    try
    {
        assert(font.loadFromFile("../font/example_font.ttf"));
        text.setFont(font);
        text.setString("test");

        texture.loadFromFile(circle_file_path);
        texture.setSmooth(true);
        sprite.setTexture(texture);
        sprite.setColor(sf::Color::Green);
        sprite.setOrigin(sprite.getLocalBounds().width/2,sprite.getLocalBounds().height/2);

    }
    catch(std::exception &e)
    {
        std::cout << "error :" << e.what() << "\n";
    }

    // window.setMouseCursorVisible(false);

    sf::String buffer("abc");
    float angle = 3.0;

    window.setTitle(std::to_string(window.hasFocus()));
    window.setVerticalSyncEnabled(true);
    sf::Vector2i localPosition = sf::Mouse::getPosition(window);

    while(window.isOpen())
    {
        /* game loop stages
         * 1. Handle input - handle events from input devices and the window
         * 2. Update frame - update objects in the scene
         * 3. Render frame - render objects from the scene onto the window
         * */
        sf::Event event;
        while(window.pollEvent(event))
        {
            switch(event.type)
            {
                case sf::Event::MouseMoved:
                {
                    localPosition = sf::Mouse::getPosition(window);
                    std::cout << localPosition.x << ", " << localPosition.y<< '\n';
                    for(auto& it:stars)
                    {
                        it.new_orbit({localPosition.x, localPosition.y});
                    }
                    // circle_shape.setPosition(localPosition.x, localPosition.y);
                    //text.setPosition(localPosition.x, localPosition.y);
                    break;
                }
                case sf::Event::MouseWheelScrolled:
                {
                    if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
                        std::cout << "wheel type: vertical" << std::endl;
                    else if (event.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel)
                        std::cout << "wheel type: horizontal" << std::endl;
                    else
                        std::cout << "wheel type: unknown" << std::endl;
                    std::cout << "wheel movement: " << event.mouseWheelScroll.delta << std::endl;
                    std::cout << "mouse x: " << event.mouseWheelScroll.x << std::endl;
                    std::cout << "mouse y: " << event.mouseWheelScroll.y << std::endl;
                    break;
                }
                case sf::Event::MouseEntered:
                {
                    std::cout << "the mouse cursor has entered the window" << std::endl;
                    break;
                }

                case sf::Event::MouseLeft:
                {
                    std::cout << "the mouse cursor has left the window" << std::endl;
                    break;
                }
                case sf::Event::EventType::LostFocus:
                {
                    angle *=-1;
                    window.setTitle("lost forcus");
                    break;
                }
                case sf::Event::EventType::GainedFocus:
                {
                    angle *= -1;
                    window.setTitle("come backed");
                    break;
                }
                case sf::Event::EventType::Closed:
                {
                    window.close();
                    break;
                }

                case sf::Event::EventType::TextEntered:
                {
                    buffer += event.text.unicode;
                    std::cout << buffer.toAnsiString()<< "\n";
                    break;
                }

                case sf::Event::EventType::KeyPressed:
                    if(event.key.code == sf::Keyboard::Key::Space)
                    {
                        window.setTitle("space was pressed");
                    }
                    break;
                case sf::Event::EventType::KeyReleased:
                    if(event.key.code == sf::Keyboard::Key::Return)
                    {
                        window.setTitle(buffer);
                        buffer.clear();
                    }
                    else if(event.key.code == sf::Keyboard::Key::Escape)
                    {
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
        for(auto& it:stars)
        {
            auto next_point = it.get_next();
            if(it.getShape())
            {
                it.setPosition4Shape(next_point.x, next_point.y);
                window.draw(*it.getShape());
            }
        }

        window.display();
    }

    return 0;
}

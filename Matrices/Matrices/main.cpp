#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <iostream>

using namespace sf;
using namespace std;

 VertexArray createLine(int x1, int y1, int x2, int y2, Color color) {
    VertexArray line(Lines, 2);
    line[0].position = Vector2f(x1, y1);
    line[1].position = Vector2f(x2, y2);
    line[0].color = color;
    line[1].color = color;
    return line;
}

class LocalSpace {
    CircleShape point;
    VertexArray xAxis, yAxis;
    
public:
    LocalSpace() {
        point.setRadius(2.f);
        point.setFillColor(Color::Green); 
        point.setOrigin(point.getPosition().x + point.getRadius(), point.getPosition().y + point.getRadius());
        point.setPosition(200, 200);

        xAxis = createLine(point.getPosition().x - 50, point.getPosition().y, point.getPosition().x + 50, point.getPosition().y, Color::Red);
        yAxis = createLine(point.getPosition().x, point.getPosition().y - 50, point.getPosition().x, point.getPosition().y + 50, Color::Blue);
    }

    void draw(RenderWindow &window) {
        window.draw(point);
        window.draw(xAxis);
        window.draw(yAxis);
    }
};

class CoordinateSystem {
    VertexArray xAxis, yAxis;

public:
    CoordinateSystem(float x, float y) {
        xAxis = createLine(0, y/2, x, y/2, Color::Red);
        yAxis = createLine(x/2, 0, x/2, y, Color::Blue);
    }

    void draw(RenderWindow& window) {
        window.draw(xAxis);
        window.draw(yAxis);
    }
};

int main()
{
    RenderWindow window(VideoMode(700, 700), "SFML works!");
    
    LocalSpace ls;
    CoordinateSystem cs(window.getSize().x, window.getSize().y);

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        window.clear();
        ls.draw(window);
        cs.draw(window);
        window.display();
    }

    return 0;
}
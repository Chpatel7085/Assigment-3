#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <iostream>
#include <cmath>
#include <string>
#include <algorithm>

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
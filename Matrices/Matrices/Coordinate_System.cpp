#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <iostream>
#include <cmath>
#include <string>
#include <algorithm>
#include "Coordinate_System.h"

using namespace sf;
using namespace std; 

CoordinateSystem::CoordinateSystem(float x, float y) {
    xAxis = createLine(0, y / 2, x, y / 2, Color::Red);
    yAxis = createLine(x / 2, 0, x / 2, y, Color::Blue);
}

void CoordinateSystem::draw(RenderWindow& window) {
    window.draw(xAxis);
    window.draw(yAxis);
}


#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <iostream>
#include <cmath>
#include <string>
#include <algorithm>
#include "Local Space.h"
#include "Coordinate_System.h"
#include "Matrix.h"
#include "createLine.h"

using namespace sf;
using namespace std;
int id = 0;

int main()
{
    RenderWindow window(VideoMode(700, 700), "SFML works!");
    window.setFramerateLimit(60);
    LocalSpace ls(window.getSize().x, window.getSize().y);
    CoordinateSystem cs(window.getSize().x, window.getSize().y);
    Matrix matrix;
    int timerPoint = 0, timerAxis = 0, buttonTimer = 0;

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        //Transformation

        if (timerPoint < 25) {
            timerPoint++;
        }

        Vector2f pointPosition = ls.getPosition();
        float radius = ls.getRadius();
        if (Mouse::isButtonPressed(Mouse::Left) && timerPoint >= 25) {
            Vector2f pos = matrix.transform(Mouse::getPosition(window).x - pointPosition.x, Mouse::getPosition(window).y - pointPosition.y, pointPosition.x, pointPosition.y);
            timerPoint = 0;
            ls.update(pos);
        }

        //Inverse Transformation

        vector<pair<RectangleShape, int> > button = ls.getButtons();
        vector<Vector2f> pointCoordinates = ls.getPointCoordinates();

        if (buttonTimer < 25)
            buttonTimer++;

        if (Mouse::isButtonPressed(Mouse::Right) && buttonTimer>=25) {
            float mousePosX = Mouse::getPosition(window).x;
            float mousePosY = Mouse::getPosition(window).y;
            for (int i = 0; i < button.size(); i++) {
                if (button[i].first.getGlobalBounds().contains(mousePosX, mousePosY)) {
                    float x = abs(pointCoordinates[button[i].second].x - pointCoordinates.back().x);
                    float y = abs(pointCoordinates[button[i].second].y - pointCoordinates.back().y);
                    if (pointCoordinates.back().x < pointCoordinates[button[i].second].x)
                        x = -x;
                    if (pointCoordinates.back().y > pointCoordinates[button[i].second].y)
                        y = -y;
                    ls.updateCoordText(x, y);
                    cout << "id -> " << button[i].second << " " << pointCoordinates.size() - 1 << endl;
                    cout << pointCoordinates[button[i].second].x << " " << pointCoordinates[button[i].second].y << endl;
                    cout << pointCoordinates.back().x << " " << pointCoordinates.back().y << endl;
                    
                    break;
                }
            }
            buttonTimer = 0;
        }

        //Rotation

        if (timerAxis < 25) {
            timerAxis++;
        }

        if (Keyboard::isKeyPressed(Keyboard::Space) && timerAxis >= 25) {
            vector<Vector2f> xAxis = ls.getPositionXAxis();
            vector<Vector2f> yAxis = ls.getPositionYAxis();
            Vector2f posX1 = matrix.rotate(45.f, xAxis[0].x, xAxis[0].y, pointPosition.x + radius, pointPosition.y + radius);
            Vector2f posX2 = matrix.rotate(45.f, xAxis[1].x, xAxis[1].y, pointPosition.x + radius, pointPosition.y + radius);
            Vector2f posY1 = matrix.rotate(45.f, yAxis[0].x, yAxis[0].y, pointPosition.x + radius, pointPosition.y + radius);
            Vector2f posY2 = matrix.rotate(45.f, yAxis[1].x, yAxis[1].y, pointPosition.x + radius, pointPosition.y + radius);
            timerAxis = 0;
            ls.updateXAxis({ {posX1}, {posX2} });
            ls.updateYAxis({ {posY1}, {posY2} });
        }

        window.clear();

        ls.draw(window);
        cs.draw(window);

        window.display();
    }

    return 0;
}
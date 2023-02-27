#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <iostream>
#include <cmath>
#include <string>
#include <algorithm>
#include "Local Space.h"

using namespace sf;
using namespace std;


LocalSpace::LocalSpace(float x, float y) {

    point.setRadius(2.f);
    point.setFillColor(Color::Green);
    point.setPosition(0, 0);

    xAxis = createLine(point.getPosition().x - 50, point.getPosition().y, point.getPosition().x + 50, point.getPosition().y, Color::Red);
    yAxis = createLine(point.getPosition().x, point.getPosition().y - 50, point.getPosition().x, point.getPosition().y + 50, Color::Blue);

    singleButton.setFillColor(Color::Blue);
    singleButton.setSize(Vector2f{ 15.f, 10.f });
    singleButton.setPosition(x / 2, y / 2);

    buttons.push_back({ singleButton, id });
    pointCoordinates.push_back({ x / 2,y / 2 });
    id++;

    if (font.loadFromFile("Textures/yess.ttf")) {
        buttonText.setFont(font);
        coordText.setFont(font);
    }

    buttonText.setCharacterSize(12);
    coordText.setCharacterSize(10);
    //buttonText.setColor(Color::Red);
}

void LocalSpace::update(Vector2f& currPos) {

    point.setPosition(currPos.x, currPos.y);

    xAxis[0].position = Vector2f(point.getPosition().x - 50, point.getPosition().y + point.getRadius());
    xAxis[1].position = Vector2f(point.getPosition().x + 50, point.getPosition().y + point.getRadius());
    yAxis[0].position = Vector2f(point.getPosition().x + point.getRadius(), point.getPosition().y - 50);
    yAxis[1].position = Vector2f(point.getPosition().x + point.getRadius(), point.getPosition().y + 50);

    singleButton.setPosition(currPos);
    buttons.push_back({ singleButton, id });

    string dis = "";
    dis += "LocalSpace ";
    dis += to_string(id);
    buttonText.setString(dis);

    buttonText.setPosition(currPos.x, currPos.y + singleButton.getSize().y + 2.f);
    buttonTexts.push_back(buttonText);

    string coord = "(0,0)";
    coordText.setString(coord);

    coordText.setPosition(currPos.x, currPos.y + singleButton.getSize().y + 20.f);

    pointCoordinates.push_back(currPos);
    id++;
}

Vector2f LocalSpace::getPosition() {
    return { point.getPosition().x, point.getPosition().y };
}

float LocalSpace::getRadius() {
    return point.getRadius();
}

vector<Vector2f> LocalSpace::getPositionXAxis() {

    return { {xAxis[0].position.x, xAxis[0].position.y}, {xAxis[1].position.x, xAxis[1].position.y} };
}

vector<Vector2f> LocalSpace::getPositionYAxis() {
    return { {yAxis[0].position.x, yAxis[0].position.y}, {yAxis[1].position.x, yAxis[1].position.y} };
}

vector<pair<RectangleShape, int> > LocalSpace::getButtons() {
    return buttons;
}

void LocalSpace::updateXAxis(vector<Vector2f> currPos) {
    xAxis[0].position = currPos[0];
    xAxis[1].position = currPos[1];
}

void LocalSpace::updateYAxis(vector<Vector2f> currPos) {
    yAxis[0].position = currPos[0];
    yAxis[1].position = currPos[1];
}

void LocalSpace::updateCoordText(float x, float y) {
    string str = "(";
    str += to_string(x);
    str += ",";
    str += to_string(y);
    str += ")";
    coordText.setString(str);
}

void LocalSpace::draw(RenderWindow& window) {
    window.draw(xAxis);
    window.draw(yAxis);
    window.draw(point);

    for (int i = 0; i < buttons.size(); i++) {
        window.draw(buttons[i].first);
    }
    for (int i = 0; i < buttonTexts.size(); i++) {
        window.draw(buttonTexts[i]);
    }

    window.draw(coordText);
}

vector<Vector2f> LocalSpace::getPointCoordinates() {
    return pointCoordinates;
}

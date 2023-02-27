#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <iostream>
#include <cmath>
#include <string>
#include <algorithm>

using namespace sf;
using namespace std;
int id = 0;

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
    vector<pair<RectangleShape, int> > buttons;
    vector<Text>buttonTexts;
    RectangleShape singleButton;
    Font font;
    Text buttonText;
    vector<Vector2f>pointCoordinates;
    Text coordText;

public:
    LocalSpace(float x, float y) {

        point.setRadius(2.f);
        point.setFillColor(Color::Green);
        point.setPosition(0, 0);

        xAxis = createLine(point.getPosition().x - 50, point.getPosition().y, point.getPosition().x + 50, point.getPosition().y, Color::Red);
        yAxis = createLine(point.getPosition().x, point.getPosition().y - 50, point.getPosition().x, point.getPosition().y + 50, Color::Blue);

        singleButton.setFillColor(Color::Blue);
        singleButton.setSize(Vector2f{ 15.f, 10.f });
        singleButton.setPosition(x/2,y/2);

        buttons.push_back({ singleButton, id});
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
    void update(Vector2f& currPos) {

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

        buttonText.setPosition(currPos.x,currPos.y+singleButton.getSize().y+2.f);
        buttonTexts.push_back(buttonText);

        string coord = "(0,0)";
        coordText.setString(coord);

        coordText.setPosition(currPos.x, currPos.y + singleButton.getSize().y + 20.f);

        pointCoordinates.push_back(currPos);
        id++;
    }
    Vector2f getPosition() {
        return { point.getPosition().x, point.getPosition().y };
    }
    float getRadius() {
        return point.getRadius();
    }
    vector<Vector2f> getPositionXAxis() {

        return { {xAxis[0].position.x, xAxis[0].position.y}, {xAxis[1].position.x, xAxis[1].position.y} };
    }
    vector<Vector2f> getPositionYAxis() {
        return { {yAxis[0].position.x, yAxis[0].position.y}, {yAxis[1].position.x, yAxis[1].position.y} };
    }
    vector<pair<RectangleShape, int> >getButtons() {
        return buttons;
    }
    void updateXAxis(vector<Vector2f> currPos) {
        xAxis[0].position = currPos[0];
        xAxis[1].position = currPos[1];
    }
    void updateYAxis(vector<Vector2f> currPos) {
        yAxis[0].position = currPos[0];
        yAxis[1].position = currPos[1];
    }
    void updateCoordText(float x, float y) {
        string str = "(";
        str += to_string(x);
        str += ",";
        str += to_string(y);
        str += ")";
        coordText.setString(str);
    }
    void draw(RenderWindow& window) {
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
    vector<Vector2f> getPointCoordinates() {
        return pointCoordinates;
    }
};

class CoordinateSystem {
    VertexArray xAxis, yAxis;

public:
    CoordinateSystem(float x, float y) {
        xAxis = createLine(0, y / 2, x, y / 2, Color::Red);
        yAxis = createLine(x / 2, 0, x / 2, y, Color::Blue);
    }

    void draw(RenderWindow& window) {
        window.draw(xAxis);
        window.draw(yAxis);
    }
};


class Matrix {

    vector<vector<float> > translationMatrix(float x, float y) {
        vector<vector<float> >transMat(3, vector<float>(3, 0.f));
        transMat[0][0] = 1.f;
        transMat[1][1] = 1.f;
        transMat[2][2] = 1.f;
        transMat[0][2] = x;
        transMat[1][2] = y;
        return transMat;
    }
    vector<vector<float> > rotationMatrix(float angle) {
        vector<vector<float> >rotateMat(3, vector<float>(3, 0.f));
        rotateMat[0][0] = cos(angle);
        rotateMat[0][1] = -sin(angle);
        rotateMat[1][0] = sin(angle);
        rotateMat[1][1] = cos(angle);
        rotateMat[2][2] = 1.f;
        return rotateMat;
    }
    vector<vector<float> > columnVector(float x, float y) {
        vector<vector<float> >colVect(3, vector<float>(1, 0.f));
        colVect[0][0] = x;
        colVect[1][0] = y;
        colVect[2][0] = 1.f;
        return colVect;
    }
    vector<float> matrixMultiply(vector<vector<float> >& mat1, vector<vector<float> >& colVector) {
        vector<float>transformVect;
        for (int i = 0; i < mat1.size(); i++) {
            float curr = 0.f;
            for (int j = 0; j < mat1.size(); j++) {
                curr += colVector[j][0] * mat1[i][j];
            }
            transformVect.push_back(curr);
        }
        return transformVect;
    }
    vector<vector<float> > inverse(vector<vector<float> >& transMat) {
        transMat[2][0] = transMat[0][2];
        transMat[2][1] = transMat[1][2];
        transMat[0][2] = 0;
        transMat[1][2] = 0;
        vector<vector<float> >inverseMat(3, vector<float>(3, 0));
        inverseMat[0][0] = transMat[1][1];
        inverseMat[0][1] = -transMat[0][1];
        inverseMat[1][0] = -transMat[1][0];
        inverseMat[1][1] = transMat[0][0];
        inverseMat[2][0] = transMat[1][0] * transMat[2][1] - transMat[2][0] * transMat[1][1];
        inverseMat[2][1] = transMat[2][0] * transMat[0][1] - transMat[0][0] * transMat[2][1];
        inverseMat[2][2] = 1.f;
        return inverseMat;
    }
public:
    Vector2f transform(float x, float y, float cX, float cY) {
        vector<vector<float> >mat1 = translationMatrix(x, y);
        vector<vector<float> >colVector = columnVector(cX, cY);
        vector<float> currPos = matrixMultiply(mat1, colVector);
        return { currPos[0], currPos[1] };
    }
    Vector2f rotate(float angle, float cX, float cY, float originX, float originY) {
        cX -= originX, cY -= originY;
        angle = angle * 3.14159265f / 180.0f;
        vector<vector<float> >mat1 = rotationMatrix(angle);
        vector<vector<float> >colVector = columnVector(cX, cY);
        vector<float> currPos = matrixMultiply(mat1, colVector);
        currPos[0] += originX, currPos[1] += originY;
        return { currPos[0], currPos[1] };
    }
    Vector2f inverseTransform(float cX, float cY) {
        vector<vector<float> >inverseMat = translationMatrix(cX, cY);
        inverseMat = inverse(inverseMat);
        float newX = cX * inverseMat[0][0] + cY * inverseMat[1][0] + inverseMat[2][0];
        float newY = cX * inverseMat[0][1] + cY * inverseMat[1][1] + inverseMat[2][1];
        return { newX, newY };
    }
};

int main()
{
    RenderWindow window(VideoMode(700, 700), "SFML works!");
    window.setFramerateLimit(60);
    LocalSpace ls(window.getSize().x, window.getSize().y);
    CoordinateSystem cs(window.getSize().x, window.getSize().y);
    Matrix matrix;
    int timerPoint = 0, timerAxis = 0;
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }
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

        vector<pair<RectangleShape, int> > button = ls.getButtons();
        vector<Vector2f> pointCoordinates = ls.getPointCoordinates();

        if (Mouse::isButtonPressed(Mouse::Right)) {
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
        }
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

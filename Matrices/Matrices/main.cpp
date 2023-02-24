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
        point.setPosition(0, 0);

        xAxis = createLine(point.getPosition().x - 50, point.getPosition().y, point.getPosition().x + 50, point.getPosition().y, Color::Red);
        yAxis = createLine(point.getPosition().x, point.getPosition().y - 50, point.getPosition().x, point.getPosition().y + 50, Color::Blue);
    }
    void update(Vector2f &currPos) {
        point.setPosition(currPos);
        xAxis[0].position = Vector2f(point.getPosition().x - 50, point.getPosition().y);
        xAxis[1].position = Vector2f(point.getPosition().x + 50, point.getPosition().y);
        yAxis[0].position = Vector2f(point.getPosition().x , point.getPosition().y - 50);
        yAxis[1].position = Vector2f(point.getPosition().x , point.getPosition().y + 50);
        point.setOrigin(point.getPosition().x + point.getRadius(), point.getPosition().y + point.getRadius());

    }
    Vector2f getPosition() {
        return { point.getPosition().x, point.getPosition().y };
    }
    void draw(RenderWindow& window) {
        window.draw(point);
        window.draw(xAxis);
        window.draw(yAxis);
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
public:
    vector<vector<float> > translationMatrix(float x, float y) {
        vector<vector<float> >transMat(3, vector<float>(3, 0.f));
        transMat[0][0] = 1.f;
        transMat[1][1] = 1.f;
        transMat[2][2] = 1.f;
        transMat[0][2] = x;
        transMat[1][2] = y;
        return transMat;
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
        for (int i = 0; i < mat1.size()-1; i++) {
            float curr = 0.f;
            for (int j = 0; j < mat1.size(); j++) {
                curr += colVector[j][0] * mat1[i][j];
            }
            transformVect.push_back(curr);
        }
        return transformVect;
    }
    Vector2f trasform(float x, float y, float cX, float cY) {
        vector<vector<float> >mat1 = translationMatrix(x, y);
        vector<vector<float> >colVector = columnVector(cX, cY);
        vector<float> currPos = matrixMultiply(mat1, colVector);
        return { currPos[0], currPos[1] };
    }

    void inverseTransform() {
        cout << "Inverse Transform" << endl;
    }
};

int main()
{
    RenderWindow window(VideoMode(700, 700), "SFML works!");
    window.setFramerateLimit(60);
    LocalSpace ls;
    CoordinateSystem cs(window.getSize().x, window.getSize().y);
    //Position position(window.getPosition().x / 2, window.getPosition().y / 2);
    Matrix matrix;
    int timer = 0;
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }
        if (timer <25) {
            timer++;
        }
        Vector2f pointPosition = ls.getPosition();
        if (Mouse::isButtonPressed(Mouse::Left) && timer >= 25) {
            Vector2f pos = matrix.trasform(Mouse::getPosition(window).x -pointPosition.x, Mouse::getPosition(window).y - pointPosition.y, pointPosition.x, pointPosition.y);
            timer = 0;
            ls.update(pos);
        }
        //cout<<ls.point
        if (Keyboard::isKeyPressed(Keyboard::Space))
            matrix.inverseTransform();

        window.clear();

        ls.draw(window);
        cs.draw(window);

        window.display();
    }

    return 0;
}
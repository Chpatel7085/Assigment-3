#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <iostream>
#include <cmath>
#include <string>
#include <algorithm>

using namespace sf;
using namespace std;

VertexArray createLine(int x1, int y1, int x2, int y2, Color color);

extern int id;
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
    LocalSpace(float x, float y);
    void update(Vector2f& currPos);
    Vector2f getPosition();
    float getRadius();
    vector<Vector2f> getPositionXAxis();
    vector<Vector2f> getPositionYAxis();
    vector<pair<RectangleShape, int> >getButtons();
    void updateXAxis(vector<Vector2f> currPos);
    void updateYAxis(vector<Vector2f> currPos);
    void updateCoordText(float x, float y);
    void draw(RenderWindow& window);
    vector<Vector2f> getPointCoordinates();
};

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <iostream>
#include <cmath>
#include <string>
#include <algorithm>

using namespace sf;
using namespace std;

class Matrix {

    vector<vector<float> > translationMatrix(float x, float y);

    vector<vector<float> > rotationMatrix(float angle);

    vector<vector<float> > columnVector(float x, float y);

    vector<float> matrixMultiply(vector<vector<float> >& mat1, vector<vector<float> >& colVector);

    vector<vector<float> > inverse(vector<vector<float> >& transMat);

public:
    Vector2f transform(float x, float y, float cX, float cY);

    Vector2f rotate(float angle, float cX, float cY, float originX, float originY);

    Vector2f inverseTransform(float cX, float cY);
};
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <iostream>
#include <cmath>
#include <string>
#include <algorithm>
#include "Matrix.h"

using namespace sf;
using namespace std;

vector<vector<float> > Matrix::translationMatrix(float x, float y) {
    vector<vector<float> >transMat(3, vector<float>(3, 0.f));
    transMat[0][0] = 1.f;
    transMat[1][1] = 1.f;
    transMat[2][2] = 1.f;
    transMat[0][2] = x;
    transMat[1][2] = y;
    return transMat;
}
vector<vector<float> > Matrix::rotationMatrix(float angle) {
    vector<vector<float> >rotateMat(3, vector<float>(3, 0.f));
    rotateMat[0][0] = cos(angle);
    rotateMat[0][1] = -sin(angle);
    rotateMat[1][0] = sin(angle);
    rotateMat[1][1] = cos(angle);
    rotateMat[2][2] = 1.f;
    return rotateMat;
}
vector<vector<float> > Matrix::columnVector(float x, float y) {
    vector<vector<float> >colVect(3, vector<float>(1, 0.f));
    colVect[0][0] = x;
    colVect[1][0] = y;
    colVect[2][0] = 1.f;
    return colVect;
}
vector<float> Matrix::matrixMultiply(vector<vector<float> >& mat1, vector<vector<float> >& colVector) {
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
vector<vector<float> > Matrix::inverse(vector<vector<float> >& transMat) {
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

Vector2f Matrix::transform(float x, float y, float cX, float cY) {
    vector<vector<float> >mat1 = translationMatrix(x, y);
    vector<vector<float> >colVector = columnVector(cX, cY);
    vector<float> currPos = matrixMultiply(mat1, colVector);
    return { currPos[0], currPos[1] };
}

Vector2f Matrix::rotate(float angle, float cX, float cY, float originX, float originY) {
    cX -= originX, cY -= originY;
    angle = angle * 3.14159265f / 180.0f;
    vector<vector<float> >mat1 = rotationMatrix(angle);
    vector<vector<float> >colVector = columnVector(cX, cY);
    vector<float> currPos = matrixMultiply(mat1, colVector);
    currPos[0] += originX, currPos[1] += originY;
    return { currPos[0], currPos[1] };
}

Vector2f Matrix::inverseTransform(float cX, float cY) {
    vector<vector<float> >inverseMat = translationMatrix(cX, cY);
    inverseMat = inverse(inverseMat);
    float newX = cX * inverseMat[0][0] + cY * inverseMat[1][0] + inverseMat[2][0];
    float newY = cX * inverseMat[0][1] + cY * inverseMat[1][1] + inverseMat[2][1];
    return { newX, newY };
}
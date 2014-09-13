#include "Utils/MatrixFactory.h"

glm::mat4 MatrixFactory::translationMatrix(float x, float y, float z)
{
    glm::mat4 translationMatrix(1.f);

    translationMatrix[3] = glm::vec4(x, y, z, 1);

    return translationMatrix;
}

glm::mat4 MatrixFactory::rotationMatrix(float x, float y, float z)
{
    glm::mat4 rotationMatrix;

    return rotationMatrix;
}

glm::mat4 MatrixFactory::scaleMatrix(float x, float y, float z)
{
    glm::mat4 scaleMatrix;

    scaleMatrix[0][0] = x;
    scaleMatrix[1][1] = y;
    scaleMatrix[2][2] = z;
    scaleMatrix[3][3] = 1.f;

    return scaleMatrix;
}
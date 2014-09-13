#ifndef MATRIX_FACTORY_H
#define MATRIX_FACTORY_H

#include "glm.hpp"

class MatrixFactory
{
public:
    static glm::mat4 translationMatrix(float x, float y, float z);
    static glm::mat4 rotationMatrix(float x, float y, float z);
    static glm::mat4 scaleMatrix(float x, float y, float z);
};

#endif // MATRIX_FACTORY_H
#include "Transformations/MatrixStack.h"

namespace
{
    inline float degToRad(float degrees)
    {
        const float degToRad = 3.141592f / 180.0f;
        return degrees * degToRad;
    }
}

MatrixStack::MatrixStack()
    : m_currentMatrix(1.0f)
{
}

const glm::mat4& MatrixStack::top()
{
    return m_currentMatrix;
}

void MatrixStack::rotateX(float degrees)
{
    m_currentMatrix = m_currentMatrix * glm::mat4(rotateXpriv(degrees));
}

void MatrixStack::rotateY(float degrees)
{
    m_currentMatrix = m_currentMatrix * glm::mat4(rotateYpriv(degrees));
}

void MatrixStack::rotateZ(float degrees)
{
    m_currentMatrix = m_currentMatrix * glm::mat4(rotateZpriv(degrees));
}

void MatrixStack::scale(const glm::vec3& scaleVec)
{
    glm::mat4 scaleMat(1.0f);
    scaleMat[0].x = scaleVec.x;
    scaleMat[1].y = scaleVec.y;
    scaleMat[2].z = scaleVec.z;

    m_currentMatrix = m_currentMatrix * scaleMat;
}

void MatrixStack::translate(const glm::vec3& offsetVec)
{
    glm::mat4 translateMat(1.0f);
    translateMat[3] = glm::vec4(offsetVec, 1.0f);

    m_currentMatrix = m_currentMatrix * translateMat;
}

void MatrixStack::push()
{
    m_matrices.push(m_currentMatrix);
}

void MatrixStack::pop()
{
    m_currentMatrix = m_matrices.top();
    m_matrices.pop();
}

void MatrixStack::set(glm::mat4 matrix)
{
    m_currentMatrix = matrix;
}

glm::mat3 MatrixStack::rotateXpriv(float degrees)
{
    float angRad = degToRad(degrees);
    float cos = cosf(angRad);
    float sin = sinf(angRad);

    glm::mat3 theMat(1.0f);
    theMat[1].y = cos; theMat[2].y = -sin;
    theMat[1].z = sin; theMat[2].z = cos;
    return theMat;
}

glm::mat3 MatrixStack::rotateYpriv(float degrees)
{
    float angRad = degToRad(degrees);
    float cos = cosf(angRad);
    float sin = sinf(angRad);

    glm::mat3 theMat(1.0f);
    theMat[0].x = cos; theMat[2].x = sin;
    theMat[0].z = -sin; theMat[2].z = cos;
    return theMat;
}

glm::mat3 MatrixStack::rotateZpriv(float degrees)
{
    float angRad = degToRad(degrees);
    float cos = cosf(angRad);
    float sin = sinf(angRad);

    glm::mat3 theMat(1.0f);
    theMat[0].x = cos; theMat[1].x = -sin;
    theMat[0].y = sin; theMat[1].y = cos;
    return theMat;
}
#ifndef MATRIX_STACK_H
#define MATRIX_STACK_H

#include <stack>
#include "glm.hpp"

class MatrixStack
{
public:
    MatrixStack();

    const glm::mat4& top();

    void rotateX(float degrees);
    void rotateY(float degrees);
    void rotateZ(float degrees);

    void scale(const glm::vec3 &scaleVec);

    void translate(const glm::vec3 &offsetVec);

    void push();
    void pop();

private:
    glm::mat3 rotateXpriv(float degrees);
    glm::mat3 rotateYpriv(float degrees);
    glm::mat3 rotateZpriv(float degrees);

    glm::mat4 m_currentMatrix;
    std::stack<glm::mat4> m_matrices;
};

#endif
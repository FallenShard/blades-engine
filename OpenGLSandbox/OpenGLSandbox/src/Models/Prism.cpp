#include "Models/Prism.h"

Prism::Prism()
{
}

Prism::~Prism()
{
}

void Prism::render()
{
    m_vertexArray->bind();
    m_vertexArray->renderIndexed();
}
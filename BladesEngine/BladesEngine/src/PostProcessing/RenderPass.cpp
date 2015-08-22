#include "PostProcessing/RenderPass.h"

namespace fsi
{
    const GLfloat RenderPass::defaultDepth = 1.f;
    const GLfloat RenderPass::defaultColor[4] = { 0.1f, 0.1f, 0.1f, 1.f };

    void RenderPass::clearScreen()
    {
        glClearNamedFramebufferfv(0, GL_COLOR, 0, defaultColor);
        glClearNamedFramebufferfv(0, GL_DEPTH, 0, &defaultDepth);
    }
}
#pragma once

namespace fsi
{
    class RenderPass
    {
    public:
        virtual void setAsSurface() = 0;
        virtual void render() = 0;
        virtual void resize(int width, int height) = 0;

        virtual ~RenderPass() = 0 {}
    };
}
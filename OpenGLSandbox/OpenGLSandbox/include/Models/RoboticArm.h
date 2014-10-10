#include "Models/SceneNode.h"


class RoboticArm : public SceneNode
{
public:
    RoboticArm();

    ~RoboticArm();

    virtual void render();

private:
    void buildHierarchy();

};
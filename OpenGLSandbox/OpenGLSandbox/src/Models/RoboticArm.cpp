#include "Models/RoboticArm.h"
#include "Models/Prism.h"

#include "gtc/matrix_transform.hpp"

RoboticArm::RoboticArm()
{
    buildHierarchy();
}

RoboticArm::~RoboticArm()
{
}

void RoboticArm::render()
{

}

void RoboticArm::buildHierarchy()
{
    //translate(glm::vec3(3.f, -5.f, -80.f));
    //rotateY(-45.f);
    SceneNode* box = new Prism();

    attachChild(std::make_shared<SceneNode>(box));

    /*
    SceneNode* leftBase = new Prism();
    leftBase->translate(glm::vec3(2.0f, 0.0f, 0.0f));
    leftBase->scale(glm::vec3(1.f, 1.f, 3.f));
    attachChild(std::make_shared<SceneNode>(leftBase));

    SceneNode* rightBase = new Prism();
    rightBase->translate(glm::vec3(-2.0f, 0.0f, 0.0f));
    rightBase->scale(glm::vec3(1.f, 1.f, 3.f));
    attachChild(std::make_shared<SceneNode>(rightBase));
    
    SceneNode* upperArm = new Prism();
    upperArm->rotateX(-33.75f);
    upperArm->translate(glm::vec3(0.f, 0.f, (9.f / 2.0f) - 1.0f));
    upperArm->scale(glm::vec3(1.f, 1.f, 9.f / 2.f));
    attachChild(std::make_shared<SceneNode>(upperArm));

    SceneNode* lowerArm = new Prism();
    lowerArm->translate(glm::vec3(0.0f, 0.0f, 8.0f));
    lowerArm->rotateX(146.25f);
    lowerArm->translate(glm::vec3(0.0f, 0.0f, 5.f / 2.0f));
    lowerArm->scale(glm::vec3(1.5f / 2.0f, 1.5f / 2.0f, 5.f / 2.0f));
    upperArm->attachChild(std::make_shared<SceneNode>(lowerArm));*/
}
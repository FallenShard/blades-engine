#include "Scenes/RobotArmScene.h"

#include "Models/SceneNode.h"
#include "Models/Prism.h"
#include "Models/RoboticArm.h"
#include "Models/Sphere.h"

#include "Renderer/ShaderManager.h"

#include <algorithm>

namespace
{
    GLenum error = false;

    RoboticArm* g_root = nullptr;
    Prism* g_prism = nullptr;
    Sphere* g_sphere = nullptr;

    ShaderManager* manager = nullptr;
    ShaderProgram* prog = nullptr;
}

RobotArmScene::RobotArmScene()
    : m_timePassed(0.f)
    , m_planeGrid(1000.f, 10.f, PlaneGrid::XZ)
    , m_cameraController(nullptr, nullptr)
{
}

RobotArmScene::RobotArmScene(Window* window)
    : m_timePassed(0.f)
    , m_planeGrid(1000.f, 10.f, PlaneGrid::XZ)
    , m_cameraController(nullptr, window)
{
}

RobotArmScene::~RobotArmScene()
{
    delete g_prism;
    delete g_root;
    delete g_sphere;
    delete manager;
}

void RobotArmScene::prepare()
{
    manager = new ShaderManager();

    prog = manager->getProgram("perspective");

    error = glGetError();
    
    prog->getUniformAttribute("modelToWorldMatrix");
    prog->getUniformAttribute("worldToCameraMatrix");
    prog->getUniformAttribute("cameraToClipMatrix");

    m_cameraController.setShaderProgram(prog);
    m_cameraController.setPosition(glm::vec3(0.f, 32.f, 36.f));

    m_planeGrid.setProgram(prog);

    m_vertexArrays["RobotArm"] = std::make_unique<VertexArray>();
    VertexArray* vArray = m_vertexArrays["RobotArm"].get();
    vArray->bind();

    m_vertexBuffers["RobotArm"] = std::make_unique<VertexBuffer>(GL_STATIC_DRAW);
    VertexBuffer* buffer = m_vertexBuffers["RobotArm"].get();
    buffer->bind();
    VertexLoader vLoader;
    vLoader.loadFromFile("res/RobotArmMesh.txt", *buffer);
    vArray->setVertexCount(buffer->getVertexCount());

    m_indexBuffers["RobotArm"] = std::make_unique<IndexBuffer>();
    IndexBuffer* indexBuffer = m_indexBuffers["RobotArm"].get();
    indexBuffer->bind();
    indexBuffer->loadFromFile("res/RobotArmFaces.txt");
    vArray->attachIndexBuffer(indexBuffer);

    vArray->attachAttribute(VertexAttribute("vPosition", 3, 0, 0));
    vArray->attachAttribute(VertexAttribute("vColor", 4, 0, 3 * sizeof(GLfloat) * buffer->getVertexCount()));
    vArray->enableAttributes(prog->getProgramId());

    m_vertexArrays["Cube"] = std::make_unique<VertexArray>(GL_LINE_LOOP);
    VertexArray* cArray = m_vertexArrays["Cube"].get();
    cArray->bind();

    m_vertexBuffers["Cube"] = std::make_unique<VertexBuffer>(GL_STATIC_DRAW);
    VertexBuffer* vBuffer = m_vertexBuffers["Cube"].get();
    vBuffer->bind();
    vLoader.loadFromFile("res/RobotArmMesh.txt", *vBuffer);
    cArray->setVertexCount(vBuffer->getVertexCount());

    m_indexBuffers["Cube"] = std::make_unique<IndexBuffer>();
    IndexBuffer* indBuffer = m_indexBuffers["Cube"].get();
    indBuffer->bind();
    indBuffer->loadFromFile("res/RobotArmFaces.txt");
    cArray->attachIndexBuffer(indexBuffer);

    cArray->attachAttribute(VertexAttribute("vPosition", 3, 0, 0));
    cArray->attachAttribute(VertexAttribute("vColor", 4, 0, 3 * sizeof(GLfloat) * vBuffer->getVertexCount()));
    cArray->enableAttributes(prog->getProgramId());

    m_robotArm.attachVertexArray(vArray);
    m_robotArm.attachShaderProgram(prog);
    g_root = new RoboticArm(prog);
    g_prism = new Prism(cArray, prog);
    g_prism->scale(glm::vec3(1.5f, 1.5f, 1.5f));

    g_sphere = new Sphere(2.f, 20, 20, prog);
    
    g_sphere->translate(glm::vec3(10.f, 0.f, -10.f));
    //g_prism->translate(glm::vec3(0.f, 0.f, -5.f));
}

namespace
{
    float pitch = 0.f;
    float yaw = 0.f;
    float roll = 0.f;
    float inc = 10.f;
}

void RobotArmScene::handleEvents(const Event& event)
{
    m_cameraController.handleEvents(event);

    switch (event.type)
    {
    case Event::KeyPressed:
    {
        switch (event.key.code)
        {
        case Keyboard::NumPad4:
        {
            //yaw += 10.f;
            //g_root->rotateY(inc);
            //g_root->applyTransformation();
            //inc += 10.f;
            //g_prism->translate(glm::vec3(-5.f, 0.f, 0.f));
            g_sphere->setPrimitiveType(GL_TRIANGLES);
            
            break;
        }
        case Keyboard::NumPad6:
        {
            //g_prism->translate(glm::vec3(5.f, 0.f, 0.f));
            //yaw -= 10.f;
            //g_root->rotateY(-inc);
            //g_root->applyTransformation();
            g_sphere->setPrimitiveType(GL_LINE_LOOP);
            break;
        }
        case Keyboard::NumPad5:
        {
            //pitch += 10.f;
            //g_root->rotateX(-inc);
            //g_root->applyTransformation();
            //g_prism->translate(glm::vec3(0.f, 0.f, 5.f));
            g_sphere->setPrimitiveType(GL_POINTS);
            break;
        }
        case Keyboard::NumPad8:
        {
            //g_prism->translate(glm::vec3(0.f, 0.f, -5.f));
            //pitch -= 10.f;
            //g_root->rotateX(inc);
            //g_root->applyTransformation();
            break;
        }
        case Keyboard::NumPad7:
        {
            //g_prism->rotateY(9.f);
            //roll += 10.f;
            //g_root->rotateZ(inc);
            //g_root->applyTransformation();
            break;
        }
        case Keyboard::NumPad9:
        {
            //g_prism->rotateY(-9.f);
            //roll -= 10.f;
            //g_root->rotateZ(-inc);
            //g_root->applyTransformation();
            break;
        }
        };
    }
    };
}

namespace
{
    float cubeTransX = 0.f;
    float cubeTranxY = 0.f;
    float cubeSpeed = 10.f;
}

void RobotArmScene::update(float timeDelta)
{
    m_timePassed += timeDelta;

    m_cameraController.update(timeDelta);

    g_root->update(timeDelta);

    if (Keyboard::isKeyPressed(Keyboard::A))
    {
        m_robotArm.moveBase(true);
        //g_prism->rotateY(2.f);
        
    }

    if (Keyboard::isKeyPressed(Keyboard::I))
    {
        g_sphere->translate(glm::vec3(-5.f * timeDelta, 0.f, 0.f));
    }

    if (Keyboard::isKeyPressed(Keyboard::O))
    {
        g_sphere->translate(glm::vec3(5.f * timeDelta, 0.f, 0.f));
    }

    if (Keyboard::isKeyPressed(Keyboard::NumPad7))
    {
        //g_prism->rotateY(1.5f);
    }

    if (Keyboard::isKeyPressed(Keyboard::NumPad9))
    {
        //g_prism->rotateY(-1.5f);
    }

    if (Keyboard::isKeyPressed(Keyboard::NumPad4))
    {
        cubeTransX -= cubeSpeed * timeDelta;
        //g_prism->translate(glm::vec3(cubeTransX, 0.f, 0.f));
        g_prism->rotateY(3.f);
    }

    if (Keyboard::isKeyPressed(Keyboard::NumPad6))
    {
        cubeTransX += cubeSpeed * timeDelta;
        //g_prism->translate(glm::vec3(cubeTransX, 0.f, 0.f));
        g_prism->rotateY(-3.f);
    }

    if (Keyboard::isKeyPressed(Keyboard::NumPad5))
    {
        //g_prism->translate(glm::vec3(0.f, 0.f, 0.16f));
        g_prism->rotateX(-3.f);
    }

    if (Keyboard::isKeyPressed(Keyboard::NumPad2))
    {
        //g_prism->translate(glm::vec3(0.f, 0.f, 0.16f));
        g_prism->rotateX(3.f);
    }

    if (Keyboard::isKeyPressed(Keyboard::NumPad8))
    {
        g_prism->translate(glm::vec3(0.f, 0.f, -0.16f));
    }
        
    if (Keyboard::isKeyPressed(Keyboard::D))
    {
        m_robotArm.moveBase(false);
        //g_prism->rotateY(-2.f);
    }
        
    if (Keyboard::isKeyPressed(Keyboard::W))
    {
        m_robotArm.moveUpperArm(false);
        g_prism->translate(glm::vec3(0.f, 0.f, -0.5f));
    }
        
    if (Keyboard::isKeyPressed(Keyboard::S))
    {
        m_robotArm.moveUpperArm(true);
        g_prism->translate(glm::vec3(0.f, 0.f, 0.5f));
    }
        
    if (Keyboard::isKeyPressed(Keyboard::R))
    {
        m_robotArm.moveLowerArm(false);
    }
        
    if (Keyboard::isKeyPressed(Keyboard::F))
    {
        m_robotArm.moveLowerArm(true);
    }
        
    if (Keyboard::isKeyPressed(Keyboard::T))
    {
        m_robotArm.moveWristPitch(false);
    }
        
    if (Keyboard::isKeyPressed(Keyboard::G))
    {
        m_robotArm.moveWristPitch(true);
    }
        
    if (Keyboard::isKeyPressed(Keyboard::Z))
    {
        m_robotArm.moveWristRoll(true);
    }
        
    if (Keyboard::isKeyPressed(Keyboard::C))
    {
        m_robotArm.moveWristRoll(false);
    }
        
    if (Keyboard::isKeyPressed(Keyboard::Q))
    {
        m_robotArm.moveFingerOpen(true);
    }
        
    if (Keyboard::isKeyPressed(Keyboard::E))
    {
        m_robotArm.moveFingerOpen(false);
    }
}

void RobotArmScene::render()
{
    prog->use();
    prog->setUniformAttribute("modelToWorldMatrix", 1, GL_FALSE, glm::value_ptr(glm::mat4(1.f)));
    
    m_planeGrid.render();
    

    //m_vertexArrays["Cube"]->bind();
    //m_vertexArrays["Cube"]->renderIndexed();

    g_root->render();
    g_prism->render();
    g_sphere->render();
    m_robotArm.render();
}

bool RobotArmScene::reshape(int width, int height)
{
    m_cameraController.resize(width, height);
    return true;
}
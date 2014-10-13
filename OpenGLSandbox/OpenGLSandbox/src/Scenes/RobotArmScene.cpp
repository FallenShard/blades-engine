#include "Scenes/RobotArmScene.h"

#include "Models/SceneNode.h"
#include "Models/Prism.h"
#include "Models/RoboticArm.h"

#include <algorithm>

namespace
{
    GLenum error = false;

    SceneNode* g_root = nullptr;
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
    delete g_root;
}

void RobotArmScene::prepare()
{
    m_shaderPrograms["RobotArm"] = std::make_unique<ShaderProgram>();
    ShaderProgram* program = m_shaderPrograms["RobotArm"].get();
    program->attachShader(Shader::Vert, "motionVert.vert");
    program->attachShader(Shader::Frag, "default.frag");
    program->link();
    program->checkLinkStatus();

    error = glGetError();

    program->getUniformAttribute("modelToWorldMatrix");
    program->getUniformAttribute("worldToCameraMatrix");
    program->getUniformAttribute("cameraToClipMatrix");

    m_cameraController.setShaderProgram(program);

    program->use();
    program->setUniformAttribute("cameraToClipMatrix", 1, GL_FALSE, glm::value_ptr(m_freeCamera.getProjectionMatrix()));

    ShaderProgram::release();

    m_planeGrid.setProgram(program);

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
    vArray->enableAttributes(program->getProgramId());

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
    cArray->enableAttributes(program->getProgramId());

    g_root = new RoboticArm(program);
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
            yaw += 10.f;
            g_root->rotateY(inc);
            //g_root->applyTransformation();
            break;
        }
        case Keyboard::NumPad6:
        {
            yaw -= 10.f;
            g_root->rotateY(-inc);
            //g_root->applyTransformation();
            break;
        }
        case Keyboard::NumPad5:
        {
            pitch += 10.f;
            g_root->rotateX(-inc);
            //g_root->applyTransformation();
            break;
        }
        case Keyboard::NumPad8:
        {
            pitch -= 10.f;
            g_root->rotateX(inc);
            //g_root->applyTransformation();
            break;
        }
        case Keyboard::NumPad7:
        {
            roll += 10.f;
            g_root->rotateZ(inc);
            //g_root->applyTransformation();
            break;
        }
        case Keyboard::NumPad9:
        {
            roll -= 10.f;
            g_root->rotateZ(-inc);
            //g_root->applyTransformation();
            break;
        }
        };

    }

    };
}

void RobotArmScene::update(float timeDelta)
{
    m_timePassed += timeDelta;

    m_cameraController.update(timeDelta);

    if (Keyboard::isKeyPressed(Keyboard::A))
        m_robotArm.moveBase(true);
    if (Keyboard::isKeyPressed(Keyboard::D))
        m_robotArm.moveBase(false);
    if (Keyboard::isKeyPressed(Keyboard::W))
        m_robotArm.moveUpperArm(false);
    if (Keyboard::isKeyPressed(Keyboard::S))
        m_robotArm.moveUpperArm(true);
    if (Keyboard::isKeyPressed(Keyboard::R))
        m_robotArm.moveLowerArm(false);
    if (Keyboard::isKeyPressed(Keyboard::F))
        m_robotArm.moveLowerArm(true);
    if (Keyboard::isKeyPressed(Keyboard::T))
        m_robotArm.moveWristPitch(false);
    if (Keyboard::isKeyPressed(Keyboard::G))
        m_robotArm.moveWristPitch(true);
    if (Keyboard::isKeyPressed(Keyboard::Z))
        m_robotArm.moveWristRoll(true);
    if (Keyboard::isKeyPressed(Keyboard::C))
        m_robotArm.moveWristRoll(false);
    if (Keyboard::isKeyPressed(Keyboard::Q))
        m_robotArm.moveFingerOpen(true);
    if (Keyboard::isKeyPressed(Keyboard::E))
        m_robotArm.moveFingerOpen(false);
}

void RobotArmScene::render()
{
    ShaderProgram* program = m_shaderPrograms["RobotArm"].get();
    VertexArray* vArray = m_vertexArrays["RobotArm"].get();
    VertexArray* gridArray = m_vertexArrays["Grid"].get();

    program->use();
    program->setUniformAttribute("modelToWorldMatrix", 1, GL_FALSE, glm::value_ptr(glm::mat4(1.f)));
    
    m_planeGrid.render();

    m_vertexArrays["Cube"]->bind();
    m_vertexArrays["Cube"]->renderIndexed();

    g_root->render();

    vArray->bind();
    m_robotArm.draw(*vArray, *program);
}

bool RobotArmScene::reshape(int width, int height)
{
    m_cameraController.resize(width, height);
    return true;
}
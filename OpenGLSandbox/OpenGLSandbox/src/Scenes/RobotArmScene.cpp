#include "Scenes/RobotArmScene.h"
#include "Renderer/Camera.h"

namespace
{
    GLshort indexData[] =
    {
        0, 1, 2,
        2, 3, 0,

        4, 5, 6,
        6, 7, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20,
    };

    bool aPressed = false;
    bool dPressed = false;
    bool wPressed = false;
    bool sPressed = false;
    bool rPressed = false;
    bool fPressed = false;
    bool tPressed = false;
    bool gPressed = false;
    bool zPressed = false;
    bool cPressed = false;
    bool qPressed = false;
    bool ePressed = false;

    GLenum error;
    glm::mat4 cameraToClipMatrix(0.0f);

    float CalcFrustumScale(float fFovDeg)
    {
        const float degToRad = 3.14159f * 2.0f / 360.0f;
        float fFovRad = fFovDeg * degToRad;
        return 1.0f / tan(fFovRad / 2.0f);
    }

    const float fFrustumScale = CalcFrustumScale(45.0f);
}

RobotArmScene::RobotArmScene()
    : m_timePassed(0.f)
    , m_camera(45.f)
{
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

    program->getUniformAttribute("modelToCameraMatrix");

    program->getUniformAttribute("cameraToClipMatrix");

    float fzNear = 1.0f; float fzFar = 100.0f;
    cameraToClipMatrix[0].x = fFrustumScale;
    cameraToClipMatrix[1].y = fFrustumScale;
    cameraToClipMatrix[2].z = (fzFar + fzNear) / (fzNear - fzFar);
    cameraToClipMatrix[2].w = -1.0f;
    cameraToClipMatrix[3].z = (2 * fzFar * fzNear) / (fzNear - fzFar);

    program->use();
    program->setUniformAttribute("cameraToClipMatrix", 1, GL_FALSE, glm::value_ptr(cameraToClipMatrix));
    ShaderProgram::release();

    //m_camera.set("cameraToClipMatrix", *program);

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
    indexBuffer->create(indexData, sizeof(indexData) / sizeof(GLshort));
    vArray->attachIndexBuffer(indexBuffer);
    
    vArray->attachAttribute(VertexAttribute("vPosition", 3, 0, 0));
    vArray->attachAttribute(VertexAttribute("vColor", 4, 0, 3 * sizeof(GLfloat) * buffer->getVertexCount()));
    vArray->enableAttributes(program->getProgramId());

    //VertexArray::release();
}


void RobotArmScene::handleEvents(const Event& event)
{
    switch (event.type)
    {
    case Event::KeyPressed:
        switch (event.key.code)
        {
        case Keyboard::A:
            aPressed = true;
            break;

        case Keyboard::D:
            dPressed = true;
            break;

        case Keyboard::W:
            wPressed = true;
            break;

        case Keyboard::S:
            sPressed = true;
            break;

        case Keyboard::R:
            rPressed = true;
            break;

        case Keyboard::F:
            fPressed = true;
            break;

        case Keyboard::T:
            tPressed = true;
            break;

        case Keyboard::G:
            gPressed = true;
            break;

        case Keyboard::Z:
            zPressed = true;
            break;

        case Keyboard::C:
            cPressed = true;
            break;

        case Keyboard::Q:
            qPressed = true;
            break;

        case Keyboard::E:
            ePressed = true;
            break;
        };
        break;

    case Event::KeyReleased:
        switch (event.key.code)
        {
        case Keyboard::A:
            aPressed = false;
            break;

        case Keyboard::D:
            dPressed = false;
            break;

        case Keyboard::W:
            wPressed = false;
            break;

        case Keyboard::S:
            sPressed = false;
            break;

        case Keyboard::R:
            rPressed = false;
            break;

        case Keyboard::F:
            fPressed = false;
            break;

        case Keyboard::T:
            tPressed = false;
            break;

        case Keyboard::G:
            gPressed = false;
            break;

        case Keyboard::Z:
            zPressed = false;
            break;

        case Keyboard::C:
            cPressed = false;
            break;

        case Keyboard::Q:
            qPressed = false;
            break;

        case Keyboard::E:
            ePressed = false;
            break;
        };
        break;
    };

}

void RobotArmScene::update(float timeDelta)
{
    m_timePassed += timeDelta;

    if (aPressed)
        m_robotArm.moveBase(true);
    if (dPressed)
        m_robotArm.moveBase(false);
    if (wPressed)
        m_robotArm.moveUpperArm(false);
    if (sPressed)
        m_robotArm.moveUpperArm(true);
    if (rPressed)
        m_robotArm.moveLowerArm(false);
    if (fPressed)
        m_robotArm.moveLowerArm(true);
    if (tPressed)
        m_robotArm.moveWristPitch(false);
    if (gPressed)
        m_robotArm.moveWristPitch(true);
    if (zPressed)
        m_robotArm.moveWristRoll(true);
    if (cPressed)
        m_robotArm.moveWristRoll(false);
    if (qPressed)
        m_robotArm.moveFingerOpen(true);
    if (ePressed)
        m_robotArm.moveFingerOpen(false);
}

void RobotArmScene::render()
{
    ShaderProgram* program = m_shaderPrograms["RobotArm"].get();
    VertexArray* vArray = m_vertexArrays["RobotArm"].get();

    program->use();
    vArray->bind();

    m_robotArm.draw(*vArray, *program);

    error = glGetError();
}

bool RobotArmScene::reshape(int width, int height)
{
    m_camera.adjustViewport(width, height, *(m_shaderPrograms["RobotArm"].get()));

    return true;
}
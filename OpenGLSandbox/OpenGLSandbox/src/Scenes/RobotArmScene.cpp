#include "Scenes/RobotArmScene.h"

#include <algorithm>

namespace
{
    int g_width;
    int g_height;
    const float PI = 3.141592f;

    GLenum error;

    int state = 0, oldX = 0, oldY = 0;
    float rX = 0, rY = 0, fov = 45;

    float degToRad(float degrees)
    {
        return degrees * 3.141592f / 180.f;
    }

    float CalcFrustumScale(float fFovDeg)
    {
        const float degToRad = 3.14159f * 2.0f / 360.0f;
        float fFovRad = fFovDeg * degToRad;
        return 1.0f / tan(fFovRad / 2.0f);
    }

    const float fFrustumScale = CalcFrustumScale(45.0f);

    glm::mat4 CalcLookAtMatrix(const glm::vec3 &cameraPt, const glm::vec3 &lookPt, const glm::vec3 &upPt)
    {
        glm::vec3 lookDir = glm::normalize(lookPt - cameraPt);
        glm::vec3 upDir = glm::normalize(upPt);

        glm::vec3 rightDir = glm::normalize(glm::cross(lookDir, upDir));
        glm::vec3 perpUpDir = glm::cross(rightDir, lookDir);

        glm::mat4 rotMat(1.0f);
        rotMat[0] = glm::vec4(rightDir, 0.0f);
        rotMat[1] = glm::vec4(perpUpDir, 0.0f);
        rotMat[2] = glm::vec4(-lookDir, 0.0f);

        rotMat = glm::transpose(rotMat);

        glm::mat4 transMat(1.0f);
        transMat[3] = glm::vec4(-cameraPt, 1.0f);

        return rotMat * transMat;
    }

    static bool g_bDrawLookatPoint = false;
    static glm::vec3 g_camTarget(0.0f, 0.4f, 0.0f);

    //In spherical coordinates.
    static glm::vec3 g_sphereCamRelPos(67.5f, -46.0f, 150.0f);

    glm::vec3 ResolveCamPosition()
    {
        MatrixStack tempMat;

        float phi = degToRad(g_sphereCamRelPos.x);
        float theta = degToRad(g_sphereCamRelPos.y + 90.0f);

        float fSinTheta = sinf(theta);
        float fCosTheta = cosf(theta);
        float fCosPhi = cosf(phi);
        float fSinPhi = sinf(phi);

        glm::vec3 dirToCamera(fSinTheta * fCosPhi, fCosTheta, fSinTheta * fSinPhi);
        return (dirToCamera * g_sphereCamRelPos.z) + g_camTarget;
    }
}

RobotArmScene::RobotArmScene()
    : m_timePassed(0.f)
    , m_planeGrid(1000.f, 10.f, PlaneGrid::XZ)
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

    program->getUniformAttribute("modelToWorldMatrix");
    program->getUniformAttribute("worldToCameraMatrix");
    program->getUniformAttribute("cameraToClipMatrix");

    /* CAMERA SETUP */
    glm::vec3 p = glm::vec3(0.f, 1.f, 1.f);
    m_freeCamera.setPosition(p);
    glm::vec3 look = glm::normalize(p);
    float yaw = glm::degrees(float(atan2(look.z, look.x) + PI));
    float pitch = glm::degrees(asin(look.y));
    rX = yaw;
    rY = pitch;
    m_freeCamera.rotate(rX, rY, 0);
    m_freeCamera.setSpeed(100.f);
    /*******************/

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
}

namespace
{
    Vector2Di mousePosPrev;
    Vector2Di mousePos;
    POINT mPrev = { 0, 0 };
    POINT mCurr = { 0, 0 };

    bool resetNeeded = true;
    bool panning = false;
}

void RobotArmScene::handleEvents(const Event& event)
{
    switch (event.type)
    {
    case Event::MouseButtonPressed:
        switch (event.mouseButton.button)
        {
        case Mouse::Right:
            mousePosPrev = mousePos;
            mousePos = Vector2Di(event.mouseButton.x, event.mouseButton.y);
            panning = true;
            ShowCursor(FALSE);
            break;
        }
        break;

    case Event::MouseMoved:
    {
        if (panning == true)
        {
            RECT rect;
            GetClientRect(GetActiveWindow(), &rect);
            mousePosPrev = Vector2Di((rect.right - rect.left) / 2, (rect.bottom - rect.top) / 2);
            mousePos = Vector2Di(event.mouseMove.x, event.mouseMove.y);

            rY += (mousePos.getY() - mousePosPrev.getY()) / 20.0f;

            rY = std::max(-90.f, std::min(rY, 90.f));

            rX += (mousePosPrev.getX() - mousePos.getX()) / 20.0f;
        }

        break;
    }
        break;

    case Event::MouseButtonReleased:
        switch (event.mouseButton.button)
        {
        case Mouse::Right:
            //mousePosPrev = mousePos = Vector2Di();
            panning = false;
            ShowCursor(TRUE);

            break;
        }

        break;
    };
}

void RobotArmScene::update(float timeDelta)
{
    m_timePassed += timeDelta;

    if (panning)
    {
        RECT rect;
        GetClientRect(GetActiveWindow(), &rect);
        POINT upperLeft = { rect.left, rect.top };
        POINT lowerRight = { rect.right, rect.bottom };
        ClientToScreen(GetActiveWindow(), &upperLeft);
        ClientToScreen(GetActiveWindow(), &lowerRight);
        POINT mousePos;
        GetCursorPos(&mousePos);
        ScreenToClient(GetActiveWindow(), &mousePos);
        POINT screenCenter = { (upperLeft.x + lowerRight.x) / 2, (lowerRight.y + upperLeft.y) / 2 };
        if (mousePos.x != screenCenter.x || mousePos.y != screenCenter.y)
        {
            SetCursorPos(screenCenter.x, screenCenter.y);
            m_freeCamera.rotate(rX, rY, 0);
        }
    }

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

    if (Keyboard::isKeyPressed(Keyboard::Up))
        m_freeCamera.walk(timeDelta);
    if (Keyboard::isKeyPressed(Keyboard::Down))
        m_freeCamera.walk(-timeDelta);
    if (Keyboard::isKeyPressed(Keyboard::Right))
        m_freeCamera.strafe(timeDelta);
    if (Keyboard::isKeyPressed(Keyboard::Left))
        m_freeCamera.strafe(-timeDelta);
}

void RobotArmScene::render()
{
    ShaderProgram* program = m_shaderPrograms["RobotArm"].get();
    VertexArray* vArray = m_vertexArrays["RobotArm"].get();
    VertexArray* gridArray = m_vertexArrays["Grid"].get();

    program->use();

    program->setUniformAttribute("worldToCameraMatrix", 1, GL_FALSE, glm::value_ptr(m_freeCamera.getViewMatrix()));
    program->setUniformAttribute("modelToWorldMatrix", 1, GL_FALSE, glm::value_ptr(glm::mat4(1.f)));

    m_planeGrid.render();

    vArray->bind();
    m_robotArm.draw(*vArray, *program);
}

bool RobotArmScene::reshape(int width, int height)
{
    g_width = width;
    g_height = height;
    m_freeCamera.setupProjection(45, (GLfloat)width / height);
    m_shaderPrograms["RobotArm"]->use();
    m_shaderPrograms["RobotArm"]->setUniformAttribute("cameraToClipMatrix", 1, GL_FALSE, glm::value_ptr(m_freeCamera.getProjectionMatrix()));
    ShaderProgram::release();
    
    glViewport(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
    return true;
}
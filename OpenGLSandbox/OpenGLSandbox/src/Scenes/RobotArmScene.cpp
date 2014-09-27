#include "Scenes/RobotArmScene.h"

#include "Renderer/Camera.h"


namespace
{
    const float PI = 3.141592f;
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

    program->getUniformAttribute("modelToWorldMatrix");
    program->getUniformAttribute("worldToCameraMatrix");
    program->getUniformAttribute("cameraToClipMatrix");

    /* CAMERA SETUP */
    glm::vec3 p = glm::vec3(5);
    m_freeCamera.setPosition(p);
    glm::vec3 look = glm::normalize(p);
    float yaw = glm::degrees(float(atan2(look.z, look.x) + PI));
    float pitch = glm::degrees(asin(look.y));
    rX = yaw;
    rY = pitch;
    m_freeCamera.rotate(rX, rY, 0);
    /*******************/

   // m_camera.set("cameraToClipMatrix", program);
    program->use();
    program->setUniformAttribute("cameraToClipMatrix", 1, GL_FALSE, m_camera.getProjectionMatrix());
    ShaderProgram::release();

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

    m_vertexArrays["Grid"] = std::make_unique<VertexArray>(GL_LINES);
    VertexArray* gridArray = m_vertexArrays["Grid"].get();
    gridArray->bind();

    m_vertexBuffers["Grid"] = std::make_unique<VertexBuffer>(GL_STATIC_DRAW);
    VertexBuffer* gridBuffer = m_vertexBuffers["Grid"].get();
    gridBuffer->bind();

    const int halfSquareSize = 500;
    // XZ plane
    for (int z = -halfSquareSize; z <= halfSquareSize; z+=10)
    {
        gridBuffer->push(-1 * halfSquareSize);
        gridBuffer->push(0.f);
        gridBuffer->push(1.f * z);
        gridBuffer->push(0.6f);
        gridBuffer->push(0.f);
        gridBuffer->push(0.f);
        gridBuffer->push(1.f);

        gridBuffer->push(1.f * halfSquareSize);
        gridBuffer->push(0.f);
        gridBuffer->push(1.f * z);
        gridBuffer->push(0.6f);
        gridBuffer->push(0.f);
        gridBuffer->push(0.f);
        gridBuffer->push(1.f);
    }

    
    for (int x = -halfSquareSize; x <= halfSquareSize; x+=10)
    {
        gridBuffer->push(1.f * x);
        gridBuffer->push(0.f);
        gridBuffer->push(-1.f * halfSquareSize);
        gridBuffer->push(0.6f);
        gridBuffer->push(0.f);
        gridBuffer->push(0.f);
        gridBuffer->push(1.f);

        gridBuffer->push(1.f * x);
        gridBuffer->push(0.f);
        gridBuffer->push(1.f * halfSquareSize);
        gridBuffer->push(0.6f);
        gridBuffer->push(0.f);
        gridBuffer->push(0.f);
        gridBuffer->push(1.f);
    }

    // XY plane
    for (int x = -halfSquareSize; x <= halfSquareSize; x += 10)
    {
        gridBuffer->push(1.f * x);
        gridBuffer->push(-1.f * halfSquareSize);
        gridBuffer->push(0.f);
        gridBuffer->push(0.f);
        gridBuffer->push(0.6f);
        gridBuffer->push(0.f);
        gridBuffer->push(1.f);

        gridBuffer->push(1.f * x);
        gridBuffer->push(1.f * halfSquareSize);
        gridBuffer->push(0.f);
        gridBuffer->push(0.f);
        gridBuffer->push(0.6f);
        gridBuffer->push(0.f);
        gridBuffer->push(1.f);
    }

    for (int y = -halfSquareSize; y <= halfSquareSize; y += 10)
    {
        gridBuffer->push(-1.f * halfSquareSize);
        gridBuffer->push(1.f * y);
        gridBuffer->push(0.f);
        gridBuffer->push(0.f);
        gridBuffer->push(0.6f);
        gridBuffer->push(0.f);
        gridBuffer->push(1.f);

        gridBuffer->push(1.f * halfSquareSize);
        gridBuffer->push(1.f * y);
        gridBuffer->push(0.f);
        gridBuffer->push(0.f);
        gridBuffer->push(0.6f);
        gridBuffer->push(0.f);
        gridBuffer->push(1.f);
    }

    // YZ plane
    for (int z = -halfSquareSize; z <= halfSquareSize; z += 10)
    {
        gridBuffer->push(0.f);
        gridBuffer->push(-1 * halfSquareSize);
        gridBuffer->push(1.f * z);
        gridBuffer->push(0.f);
        gridBuffer->push(0.f);
        gridBuffer->push(0.6f);
        gridBuffer->push(1.f);

        gridBuffer->push(0.f);
        gridBuffer->push(1.f * halfSquareSize);
        gridBuffer->push(1.f * z);
        gridBuffer->push(0.f);
        gridBuffer->push(0.f);
        gridBuffer->push(0.6f);
        gridBuffer->push(1.f);
    }

    for (int y = -halfSquareSize; y <= halfSquareSize; y += 10)
    {
        gridBuffer->push(0.f);
        gridBuffer->push(1.f * y);
        gridBuffer->push(-1.f * halfSquareSize);
        gridBuffer->push(0.f);
        gridBuffer->push(0.f);
        gridBuffer->push(0.6f);
        gridBuffer->push(1.f);

        gridBuffer->push(0.f);
        gridBuffer->push(1.f * y);
        gridBuffer->push(1.f * halfSquareSize);
        gridBuffer->push(0.f);
        gridBuffer->push(0.f);
        gridBuffer->push(0.6f);
        gridBuffer->push(1.f);
    }

    gridBuffer->setDataCountPerVertex(7);
    gridBuffer->uploadData();
    gridArray->setVertexCount(gridBuffer->getVertexCount());

    glPointSize(2.f);

    gridArray->attachAttribute(VertexAttribute("vPosition", 3, 7 * sizeof(GLfloat), 0));
    gridArray->attachAttribute(VertexAttribute("vColor", 4, 7 * sizeof(GLfloat), 3 * sizeof(GLfloat)));
    gridArray->enableAttributes(program->getProgramId());

    VertexArray* targetArray = new VertexArray(GL_POINTS);
    //m_vertexArrays["CameraTarget"] = std::make_unique<VertexArray>(GL_POINTS);
    m_vertexArrays["CameraTarget"] = std::unique_ptr<VertexArray>(targetArray);
    //VertexArray* targetArray = m_vertexArrays["CameraTarget"].get();
    targetArray->bind();

    m_vertexBuffers["CameraTarget"] = std::make_unique<VertexBuffer>(GL_STATIC_DRAW);
    VertexBuffer* targetBuffer = m_vertexBuffers["CameraTarget"].get();
    targetBuffer->bind();

    targetBuffer->push(0.f);
    targetBuffer->push(0.f);
    targetBuffer->push(0.f);
    targetBuffer->push(0.f);
    targetBuffer->push(0.f);
    targetBuffer->push(1.f);
    targetBuffer->push(1.f);
    targetBuffer->setDataCountPerVertex(7);
    targetBuffer->uploadData();

    targetArray->setVertexCount(targetBuffer->getVertexCount());
    targetArray->attachAttribute(VertexAttribute("vPosition", 3, 7 * sizeof(GLfloat), 0));
    targetArray->attachAttribute(VertexAttribute("vColor", 4, 7 * sizeof(GLfloat), 3 * sizeof(GLfloat)));
    targetArray->enableAttributes(program->getProgramId());
}

namespace
{
    bool camUp = false;
    bool camDown = false;

    glm::mat4 CameraMatrix(1.f);

    Vector2Di mousePosPrev;
    Vector2Di mousePos;

    int yDir = 0;
    int xDir = 0;

    bool panning = false;

    glm::vec3 target(0.f, 0.f, -10.f);
}


void RobotArmScene::handleEvents(const Event& event)
{
    switch (event.type)
    {
    case Event::MouseButtonPressed:
        switch (event.mouseButton.button)
        {
        case Mouse::Left:
            mousePosPrev = mousePos;
            mousePos = Vector2Di(event.mouseButton.x, event.mouseButton.y);
            panning = true;
            break;
            
        }
        break;

    case Event::MouseMoved:
        if (panning == true)
        {
            mousePosPrev = mousePos;
            mousePos = Vector2Di(event.mouseMove.x, event.mouseMove.y);

            yDir = mousePos.getY() - mousePosPrev.getY();
            xDir = mousePos.getX() - mousePosPrev.getX();
        }
        break;

    case Event::MouseButtonReleased:
        switch (event.mouseButton.button)
        {
        case Mouse::Left:
            mousePosPrev = mousePos = Vector2Di();
            panning = false;
            break;
        }

        break;
    };

}

void RobotArmScene::update(float timeDelta)
{
    m_timePassed += timeDelta;

    VertexBuffer* camTarget = m_vertexBuffers["CameraTarget"].get();

    if (yDir < 0 && panning)
        CameraMatrix[3].y += 0.1f;

    if (yDir > 0 && panning)
        CameraMatrix[3].y -= 0.1f;

    if (xDir < 0 && panning)
    {
        target.x -= 0.1f;
        target.y = 0.f;
        target.z = -10.f;
        (*camTarget)[0] = target.x;
        (*camTarget)[1] = target.y;
        (*camTarget)[2] = target.z;
        camTarget->uploadData();

        CameraMatrix[3].x -= 0.1f;
    }

    if (xDir > 0 && panning)
    {
        target.x += 0.1f;
        target.y = 0.f;
        target.z = -10.f;
        (*camTarget)[0] = target.x;
        (*camTarget)[1] = target.y;
        (*camTarget)[2] = target.z;
        camTarget->uploadData();

        CameraMatrix[3].x += 0.1f;
    }


    if (camUp)
    {
        //g_camTarget.z -= 0.1f;
        CameraMatrix[3].y += 0.1f;
    }

    if (camDown)
    {
        //g_camTarget.z += 0.1f;
        CameraMatrix[3].y -= 0.1f;
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
}

void RobotArmScene::render()
{
    ShaderProgram* program = m_shaderPrograms["RobotArm"].get();
    VertexArray* vArray = m_vertexArrays["RobotArm"].get();
    VertexArray* gridArray = m_vertexArrays["Grid"].get();

    error = glGetError();
    program->use();
    error = glGetError();
    glm::vec3 camPos = ResolveCamPosition();
    MatrixStack cameraMatrix;
    cameraMatrix.set(CalcLookAtMatrix(camPos, g_camTarget, glm::vec3(0.f, 1.f, 0.f)));

    //program->setUniformAttribute("worldToCameraMatrix", 1, GL_FALSE, glm::value_ptr(cameraMatrix.top()));
    //program->setUniformAttribute("worldToCameraMatrix", 1, GL_FALSE, glm::value_ptr(CameraMatrix));

    glm::mat4 mat = glm::lookAt(glm::vec3(0.f, 1.f, 0.f), target, glm::vec3(0, 1, 0));
    program->setUniformAttribute("worldToCameraMatrix", 1, GL_FALSE, glm::value_ptr(mat));

    program->setUniformAttribute("modelToWorldMatrix", 1, GL_FALSE, glm::value_ptr(glm::mat4(1.f)));
    gridArray->bind();
    gridArray->render();

    m_vertexArrays["CameraTarget"]->bind();
    m_vertexArrays["CameraTarget"]->render();

    vArray->bind();
    m_robotArm.draw(*vArray, *program);
}

bool RobotArmScene::reshape(int width, int height)
{
    m_camera.adjustViewport(width, height);
    m_shaderPrograms["RobotArm"]->use();
    m_shaderPrograms["RobotArm"]->setUniformAttribute("cameraToClipMatrix", 1, GL_FALSE, m_camera.getProjectionMatrix());
    ShaderProgram::release();
    m_freeCamera.setupProjection(45, (GLfloat)width / height);

    glViewport(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
    return true;
}
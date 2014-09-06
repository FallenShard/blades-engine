#include "Scenes/TransformationScene.h"

namespace
{
    glm::mat4 cameraToClipMatrix;
    glm::mat4 modelToCameraMatrix;

    float CalcFrustumScale(float fovDegrees)
    {
        const float degToRad = 3.14159f / 180.f;
        float fFovRad = fovDegrees * degToRad;
        return 1.0f / tan(fFovRad / 2.0f);
    }

    float fFrustumScale = CalcFrustumScale(45.f);

    struct Instance
    {
        typedef glm::vec3(*OffsetFunc)(float);

        OffsetFunc CalcOffset;

        glm::mat4 ConstructMatrix(float elapsedTime)
        {
            glm::mat4 theMat(1.0f);

            theMat[3] = glm::vec4(CalcOffset(elapsedTime), 1.0f);

            return theMat;
        }
    };

    Instance g_instanceList[3];



    glm::vec3 StationaryOffset(float elapsedTime)
    {
        return glm::vec3(0.f, 0.f, -20.f);
    }

    glm::vec3 OvalOffset(float elapsedTime)
    {
        const float loopDuration = 3.f;
        const float scale = 3.141592f * 2.f / loopDuration;

        float currentTime = fmodf(elapsedTime, loopDuration);

        return glm::vec3(cosf(currentTime * scale) * 4.f,
            sinf(currentTime * scale) * 6.f, -20.f);
    }

    glm::vec3 BottomCircleOffset(float elapsedTime)
    {
        const float loopDuration = 12.f;
        const float scale = 3.141592f * 2.f / loopDuration;

        float currentTime = fmodf(elapsedTime, loopDuration);

        return glm::vec3(cosf(currentTime * scale) * 5.f,
            -3.5f,
            sinf(currentTime * scale) * 5.f - 20.f);
    }
}

TransformationScene::TransformationScene()
    : m_timePassed(0.f)
{
}

void TransformationScene::prepare()
{
    // Create an accompanying shader program
    m_shaderPrograms["Translation"] = std::make_unique<ShaderProgram>();
    ShaderProgram* program = m_shaderPrograms["Translation"].get();
    program->attachShader(Shader::Vert, "motionVert.vert");
    program->attachShader(Shader::Frag, "default.frag");
    program->link();
    program->checkLinkStatus();

    program->getUniformAttribute("position");
    program->getUniformAttribute("color");
    program->getUniformAttribute("modelToCameraMatrix");
    program->getUniformAttribute("cameraToClipMatrix");

    float fzNear = 1.0f; float fzFar = 45.0f;

    cameraToClipMatrix[0].x = fFrustumScale;
    cameraToClipMatrix[1].y = fFrustumScale;
    cameraToClipMatrix[2].z = (fzFar + fzNear) / (fzNear - fzFar);
    cameraToClipMatrix[2].w = -1.0f;
    cameraToClipMatrix[3].z = (2 * fzFar * fzNear) / (fzNear - fzFar);

    program->use();
    program->setUniformAttribute("cameraToClipMatrix", 1, GL_FALSE, glm::value_ptr(cameraToClipMatrix));
    ShaderProgram::release();

    GLshort indexData[] =
    {
        0, 1, 2,
        1, 0, 3,
        2, 3, 0,
        3, 2, 1,

        5, 4, 6,
        4, 5, 7,
        7, 6, 4,
        6, 7, 5,
    };

    // Translation
    m_vertexArrays["Translation"] = std::make_unique<VertexArray>(GL_TRIANGLES);
    VertexArray* vArray = m_vertexArrays["Translation"].get();
    vArray->bind();

    m_vertexBuffers["Object"] = std::make_unique<VertexBuffer>(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    VertexBuffer* buffer = m_vertexBuffers["Object"].get();
    buffer->bind();
    buffer->loadFromFile("res/TranslationData.txt");
    vArray->setVertexCount(buffer->getVertexCount());

    m_indexBuffers["Object"] = std::make_unique<IndexBuffer>(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
    IndexBuffer* indexBuffer = m_indexBuffers["Object"].get();
    indexBuffer->bind();
    indexBuffer->create(indexData, sizeof(indexData) / sizeof(GLshort));
    vArray->attachIndexBuffer(indexBuffer);


    VertexBuffer::AttributeMap attributeMap = buffer->getAttributeMap();
    if (attributeMap.find("Position") != attributeMap.end())
        attributeMap["Position"]->name = "vPosition";
    if (attributeMap.find("Color") != attributeMap.end())
        attributeMap["Color"]->name = "vColor";

    for (auto& attribute : attributeMap)
    {
        attribute.second->locate(program->getProgramId());
        attribute.second->enable();
    }

    VertexArray::release();

    g_instanceList[0] = { StationaryOffset };
    g_instanceList[1] = { OvalOffset };
    g_instanceList[2] = { BottomCircleOffset };
}

void TransformationScene::handleEvents(const Event& event)
{
}

void TransformationScene::update(float timeDelta)
{
    m_timePassed += timeDelta;
}

void TransformationScene::render()
{
    VertexArray* vArray = m_vertexArrays["Translation"].get();
    ShaderProgram* program = m_shaderPrograms["Translation"].get();
    vArray->bind();
    program->use();

    for (int iLoop = 0; iLoop < 3; iLoop++)
    {
        Instance &currInst = g_instanceList[iLoop];
        const glm::mat4 &transformMatrix = currInst.ConstructMatrix(m_timePassed);

        program->setUniformAttribute("modelToCameraMatrix", 1, GL_FALSE, glm::value_ptr(transformMatrix));
        vArray->renderIndexed();
    }
}
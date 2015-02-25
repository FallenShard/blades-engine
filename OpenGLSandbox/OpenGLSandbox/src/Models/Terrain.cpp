#include "Models/Terrain.h"
#include <fstream>
#include <iostream>

#include "OglWrapper/VertexArray.h"
#include "OglWrapper/IndexBuffer.h"
#include "OglWrapper/VertexBuffer.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


namespace
{

    bool hasCompileError(GLint id)
    {
        GLint compileStatus = 0;
        bool hasError = 0;
        glGetShaderiv(id, GL_COMPILE_STATUS, &compileStatus);

        if (compileStatus == GL_FALSE)
        {
            hasError = true;
            int infoLogLength = 0;

            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
            GLchar* buffer = new GLchar[infoLogLength];
            GLsizei charsWritten = 0;
            glGetShaderInfoLog(id, infoLogLength, &charsWritten, buffer);

            std::cout << buffer << '\n';

            delete[] buffer;
        }

        return hasError;
    }

    bool hasLinkError(GLint id)
    {
        GLint linkStatus = 0;
        bool hasError = 0;
        glGetProgramiv(id, GL_LINK_STATUS, &linkStatus);

        if (linkStatus == GL_FALSE)
        {
            hasError = true;
            int infoLogLength = 0;

            glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
            GLchar* buffer = new GLchar[infoLogLength];
            GLsizei charsWritten = 0;
            glGetProgramInfoLog(id, infoLogLength, &charsWritten, buffer);

            std::cout << buffer << '\n';

            delete[] buffer;
        }

        return hasError;
    }

    void loadFile(std::string fileName, std::string& outData)
    {
        std::ifstream inputFile(fileName);

        if (inputFile.is_open())
        {
            std::string line;
            while (std::getline(inputFile, line))
                outData += line + '\n';
            inputFile.close();
        }
    }

    GLint createShader(GLenum type, std::string fileName)
    {
        GLint shaderId = glCreateShader(type);
        std::string shaderSrc = "";
        loadFile(fileName, shaderSrc);
        const GLchar* source = shaderSrc.c_str();
        glShaderSource(shaderId, 1, &source, nullptr);
        glCompileShader(shaderId);
        if (hasCompileError(shaderId))
            return -1;

        return shaderId;
    }

    GLint createProgram()
    {
        GLint vShaderId = createShader(GL_VERTEX_SHADER, "res/terrain.vert");
        GLint fShaderId = createShader(GL_FRAGMENT_SHADER, "res/terrain.frag");
        GLint tcShaderId = createShader(GL_TESS_CONTROL_SHADER, "res/terrain.tesc");
        GLint teShaderId = createShader(GL_TESS_EVALUATION_SHADER, "res/terrain.tese");

        GLint progId = glCreateProgram();

        glAttachShader(progId, vShaderId);
        glAttachShader(progId, fShaderId);
        glAttachShader(progId, tcShaderId);
        glAttachShader(progId, teShaderId);

        glLinkProgram(progId);

        if (hasLinkError(progId))
            return -1;

        glDeleteShader(vShaderId);
        glDeleteShader(fShaderId);
        glDeleteShader(tcShaderId);
        glDeleteShader(teShaderId);

        return progId;
    }
}

namespace
{
    VertexArray* vao;
    VertexBuffer* vbo;
    IndexBuffer* ibo;

    GLuint texId;
    GLuint prog;
    GLint MVPunif;
    GLint hMap;

    GLuint timeUnif;
    GLfloat timeVal = 0.f;
}

Terrain::Terrain()
{
}

Terrain::~Terrain()
{
    delete vao;
    delete vbo;
    delete ibo;
}


void Terrain::init()
{
    vao = new VertexArray(GL_PATCHES);
    vao->bind();

    vbo = new VertexBuffer(GL_STATIC_DRAW);
    ibo = new IndexBuffer();

    prog = createProgram();

    MVPunif = glGetUniformLocation(prog, "MVP");
    hMap = glGetUniformLocation(prog, "hMap");
    timeUnif = glGetUniformLocation(prog, "time");

    GLint positionLoc = glGetAttribLocation(prog, "position");
    GLint colorLoc = glGetAttribLocation(prog, "color");

    int squares = 32;
    float squareSize = 8.f;
    float netSize = 256.f;

    int i = 0;
    for (int z = 0; z < squares + 1; z++)
        for (int x = 0; x < squares + 1; x++)
            vbo->push(x * squareSize - netSize / 2, 0.f, z * squareSize - netSize / 2);

    for (int z = 0; z < squares; z++)
        for (int x = 0; x < squares; x++)
        {
            ibo->push(z * (squares + 1) + x, z * (squares + 1) + x + 1, (z + 1) * (squares + 1) + x + 1, (z + 1) * (squares + 1) + x);
        }

    vbo->bind();
    vbo->setDataCountPerVertex(3);
    vbo->uploadData();
    vao->setVertexCount(vbo->getVertexCount());

    ibo->bind();
    ibo->uploadData();
    vao->attachIndexBuffer(ibo);

    vao->attachAttribute(VertexAttribute("position", 3, 0, 0));
    vao->enableAttributes(prog);

    glPatchParameteri(GL_PATCH_VERTICES, 4);

    int w;
    int h;
    int comp;

    glUseProgram(prog);
    glUniform1i(hMap, 1);
    

    unsigned char* img = stbi_load("res/heightmap.png", &w, &h, &comp, STBI_rgb_alpha);

    glGenTextures(1, &texId);

    glBindTexture(GL_TEXTURE_2D, texId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    stbi_image_free(img);

    VertexArray::release();
}

void Terrain::update(const float deltaTime)
{
    timeVal += deltaTime;
}


void Terrain::render(const glm::mat4& projection, const glm::mat4& view)
{
    glm::mat4 model = glm::translate(glm::vec3(0.f, -50.f, 0.f));
    glm::mat4 mat = projection * view * model;

    glUseProgram(prog);

    glUniformMatrix4fv(MVPunif, 1, GL_FALSE, glm::value_ptr(mat));
    glUniform1f(timeUnif, timeVal);

    ////glDisable(GL_CULL_FACE);

    glLineWidth(1.f);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    vao->bind();

    vao->renderIndexed();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //glEnable(GL_CULL_FACE);
}
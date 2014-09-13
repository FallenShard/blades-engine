#include "Scenes/GraphScene.h"
#include <algorithm>

namespace
{
    typedef std::chrono::high_resolution_clock myclock;
    typedef std::mt19937 MyRNG;

    MyRNG rng;
    uint32_t seed_val = 23;

    const int vertexCount = 50;
    const int populationNumber = 2000;

    bool computing = false;

    struct Vertex
    {
        GLfloat x, y, z, w;

        GLfloat r, g, b, a;

        Vertex(float x1, float y1, float z1, float w1, float r1, float g1, float b1, float a1)
        {
            x = x1;
            y = y1;
            z = z1;
            w = w1;
            r = r1;
            g = g1;
            b = b1;
            a = a1;
        }
    };

    std::vector<Vertex> vertices;
    std::vector<Vertex> edges;
    std::vector<Vertex> path;

    struct Permutation
    {
        std::vector<int> permIndices;

        Permutation() {}

        Permutation(std::vector<int> indices)
        {
            permIndices = indices;
        }

        float euclideanDistance()
        {
            float distance = 0.f;
            for (unsigned int i = 0; i < vertexCount - 1; i++)
            {
                distance += (vertices[permIndices[i]].x - vertices[permIndices[i + 1]].x) * (vertices[permIndices[i]].x - vertices[permIndices[i + 1]].x) +
                            (vertices[permIndices[i]].y - vertices[permIndices[i + 1]].y) * (vertices[permIndices[i]].y - vertices[permIndices[i + 1]].y);
            }
            return distance;
        }

        void shuffle()
        {
            std::random_shuffle(permIndices.begin(), permIndices.end());
        }

        Permutation crossover(const Permutation& perm)
        {
            int randIndex = rand() % permIndices.size();
            Permutation childPerm;

            for (int i = 0; i < randIndex; i++)
                childPerm.permIndices.push_back(permIndices[i]);

            for (unsigned int i = 0; i < perm.permIndices.size(); i++)
            {
                if (std::find(childPerm.permIndices.begin(), childPerm.permIndices.end(), perm.permIndices[i]) == childPerm.permIndices.end())
                    childPerm.permIndices.push_back(perm.permIndices[i]);
            }

            return childPerm;
        }

        void mutate()
        {
            unsigned int chance = rand() % permIndices.size();
            int randIndex1 = rand() % permIndices.size();
            int randIndex2 = rand() % permIndices.size();

            if (chance > permIndices.size() / 2)
            {
                int temp = permIndices[randIndex1];
                permIndices[randIndex1] = permIndices[randIndex2];
                permIndices[randIndex2] = temp;
            }
        }
    };

    Permutation bestPerm;
    Permutation anotherPerm;

    std::vector<Permutation> population;

    GLenum errorReport = 0;
}

GraphScene::GraphScene()
    : m_timePassed(0.f)
{
    rng.seed(seed_val);
    distribution = std::uniform_real_distribution<float>(-1, 1);

    std::vector<int> defIndices;
    for (unsigned int i = 0; i < vertexCount; defIndices.push_back(i++));
    Permutation defPerm(defIndices);

    bestPerm = defPerm;
    anotherPerm = defPerm;
}

void GraphScene::prepare()
{
    // Create an accompanying shader program
    m_shaderPrograms["TSP"] = std::make_unique<ShaderProgram>();
    ShaderProgram* program = m_shaderPrograms["TSP"].get();
    program->attachShader(Shader::Vert, "default.vert");
    program->attachShader(Shader::Frag, "default.frag");
    program->link();
    program->checkLinkStatus();

    // Graph
    m_vertexArrays["TSP"] = std::make_unique<VertexArray>(GL_POINTS);
    m_vertexArrays["TSPEdges"] = std::make_unique<VertexArray>(GL_LINES);
    m_vertexArrays["TSPPath"] = std::make_unique<VertexArray>(GL_LINES);

    m_vertexBuffers["TSP"] = std::make_unique<VertexBuffer>(GL_ARRAY_BUFFER, GL_STREAM_DRAW);
    m_vertexBuffers["TSPEdges"] = std::make_unique<VertexBuffer>(GL_ARRAY_BUFFER, GL_STREAM_DRAW);
    m_vertexBuffers["TSPPath"] = std::make_unique<VertexBuffer>(GL_ARRAY_BUFFER, GL_STREAM_DRAW);

    randomize(0);

    program->use();

    glPointSize(7.f);
}

void GraphScene::handleEvents(const Event& event)
{
    if (event.type == Event::KeyPressed && event.key.code == Keyboard::S)
    {
        rng.seed(++seed_val);
        randomize(0);
    }

    if (event.type == Event::KeyPressed && event.key.code == Keyboard::C)
    {
        computing = true;
    }

    if (event.type == Event::KeyReleased && event.key.code == Keyboard::C)
    {
        computing = false;
    }
}

void GraphScene::update(float timeDelta)
{
    m_timePassed += timeDelta;

    if (computing)
        TSPIteration();
}

void GraphScene::render()
{
    ShaderProgram* program = m_shaderPrograms["TSP"].get();
    VertexArray* vArray = m_vertexArrays["TSP"].get();
    VertexArray* VAOedges = m_vertexArrays["TSPEdges"].get();
    VertexArray* VAOPath = m_vertexArrays["TSPPath"].get();

    program->use();

    //glLineWidth(1.f);
    //VAOedges->bind();
    //VAOedges->render();

    glLineWidth(3.f);
    VAOPath->bind();
    VAOPath->render();

    vArray->bind();
    vArray->render();

    errorReport = glGetError();
}

bool GraphScene::reshape(int width, int height)
{
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    return true;
}

void GraphScene::randomize(unsigned int seed)
{
    ShaderProgram* program = m_shaderPrograms["TSP"].get();

    VertexBuffer* buffer = m_vertexBuffers["TSP"].get();
    VertexArray* vArray = m_vertexArrays["TSP"].get();
    vArray->bind();
    buffer->bind();
    
    auto randomCoord = std::bind(distribution, generator);

    vertices.clear();
    for (int i = 0; i < vertexCount; i++)
    {
        vertices.push_back(Vertex(distribution(rng), distribution(rng), 0.f, 1.f, 1.f, 0.f, 1.f, 1.f));
    }
    buffer->create((GLfloat*)(vertices.data()), 8 * vertices.size());
    vArray->setVertexCount(vertices.size());

    VertexAttribute posAttrib("vPosition", 4, sizeof Vertex, 0);
    VertexAttribute colAttrib("vColor", 4, sizeof Vertex, 4 * sizeof GLfloat);
    posAttrib.enable(program->getProgramId());
    colAttrib.enable(program->getProgramId());

    

    vArray = m_vertexArrays["TSPEdges"].get();
    buffer = m_vertexBuffers["TSPEdges"].get();
    vArray->bind();
    buffer->bind();

    edges.clear();
    for (unsigned int i = 0; i < vertices.size(); i++)
    {
        for (unsigned int j = i + 1; j < vertices.size(); j++)
        {
            edges.push_back(Vertex(vertices[i].x, vertices[i].y, 0.f, 1.f, 0.3f, 0.3f, 0.3f, 1.f));
            edges.push_back(Vertex(vertices[j].x, vertices[j].y, 0.f, 1.f, 0.3f, 0.3f, 0.3f, 1.f));
        }
    }
    buffer->create((GLfloat*)(edges.data()), 8 * edges.size());
    vArray->setVertexCount(edges.size());

    posAttrib.enable(program->getProgramId());
    colAttrib.enable(program->getProgramId());

    vArray = m_vertexArrays["TSPPath"].get();
    buffer = m_vertexBuffers["TSPPath"].get();
    vArray->bind();
    buffer->bind();

    path.clear();
    for (unsigned int i = 0; i < bestPerm.permIndices.size() - 1; i++)
    {
        path.push_back(Vertex(vertices[bestPerm.permIndices[i]].x,     vertices[bestPerm.permIndices[i]].y,     0.f, 1.f, 0.0f, 1.0f, 0.0f, 1.f));
        path.push_back(Vertex(vertices[bestPerm.permIndices[i + 1]].x, vertices[bestPerm.permIndices[i + 1]].y, 0.f, 1.f, 0.0f, 1.0f, 0.0f, 1.f));
    }
    buffer->create((GLfloat*)(path.data()), 8 * path.size());
    vArray->setVertexCount(path.size());
    posAttrib.enable(program->getProgramId());
    colAttrib.enable(program->getProgramId());

    VertexArray::release();
    VertexBuffer::release(*buffer);
}

void GraphScene::TSPIteration()
{
    path.clear();
    std::vector<int> defIndices;
    for (unsigned int i = 0; i < vertexCount; defIndices.push_back(i++));
    Permutation defPerm(defIndices);

    population.clear();
    for (unsigned int i = 0; i < populationNumber; i++)
    {
        if (i % 2)
        {
            population.push_back(bestPerm.crossover(anotherPerm));
        }
        else
        {
            population.push_back(bestPerm.crossover(anotherPerm));
            population[i].mutate();
        }
    }

    for (unsigned int i = 0; i < population.size(); i++)
    {
        if (population[i].euclideanDistance() < bestPerm.euclideanDistance())
            bestPerm = population[i];
        else if (population[i].euclideanDistance() < anotherPerm.euclideanDistance())
            anotherPerm = population[i];
    }

    for (unsigned int i = 0; i < bestPerm.permIndices.size() - 1; i++)
    {
        path.push_back(Vertex(vertices[bestPerm.permIndices[i]].x, vertices[bestPerm.permIndices[i]].y, 0.f, 1.f,         0.0f, 1.0f, 0.0f, 1.f));
        path.push_back(Vertex(vertices[bestPerm.permIndices[i + 1]].x, vertices[bestPerm.permIndices[i + 1]].y, 0.f, 1.f, 0.f,  1.0f, 0.0f, 1.f));
    }

    ShaderProgram* program = m_shaderPrograms["TSP"].get();
    VertexArray* vArray = m_vertexArrays["TSPPath"].get();
    VertexBuffer*  buffer = m_vertexBuffers["TSPPath"].get();
    vArray->bind();
    buffer->bind();

    buffer->create((GLfloat*)(path.data()), 8 * path.size());
    vArray->setVertexCount(path.size());

    VertexAttribute posAttrib("vPosition", 4, sizeof Vertex, 0);
    VertexAttribute colAttrib("vColor", 4, sizeof Vertex, 4 * sizeof GLfloat);
    posAttrib.enable(program->getProgramId());
    colAttrib.enable(program->getProgramId());

    VertexArray::release();
    VertexBuffer::release(*buffer);
}
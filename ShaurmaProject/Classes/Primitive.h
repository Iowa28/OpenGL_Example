#pragma once

class Primitive
{
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

public:
    Primitive() {}
    virtual ~Primitive() {}

    void set(
        const Vertex* vertices,
        const unsigned nrOfVertices,
        const GLuint* indices,
        const unsigned nrOfIndices
    )
    {
        for (size_t i = 0; i < nrOfVertices; i++)
        {
            this->vertices.push_back(vertices[i]);
        }

        for (size_t i = 0; i < nrOfIndices; i++)
        {
            this->indices.push_back(indices[i]);
        }
    }

    Vertex* getVertices() { return this->vertices.data(); }
    GLuint* getIndices() { return this->indices.data(); }
    unsigned getNrOfVertices() { return this->vertices.size(); }
    unsigned getNrOfIndices() { return this->indices.size(); }
};

class Triangle : public Primitive
{
public:
    Triangle()
    {
        constexpr Vertex vertices[] =
        {
            vec3(-0.5f, 0.5f, 0.f), vec3(1.f, 0.f, 0.f), vec2(0.f, 1.f), vec3(0.f, 0.f, 1.f),
            vec3(-0.5f, -0.5f, 0.f), vec3(0.f, 1.f, 0.f), vec2(0.f, 0.f), vec3(0.f, 0.f, 1.f),
            vec3(0.5f, -0.5f, 0.f), vec3(0.f, 0.f, 1.f), vec2(1.f, 0.f), vec3(0.f, 0.f, 1.f),
            vec3(0.5f, 0.5f, 0.f), vec3(1.f, 1.f, 0.f), vec2(1.f, 1.f), vec3(0.f, 0.f, 1.f)
        };
        constexpr unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

        constexpr GLuint indices[] =
        {
            0, 1, 2
        };
        constexpr unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

        this->set(vertices, nrOfVertices, indices, nrOfIndices);
    }
};

class Quad : public Primitive
{
public:
    Quad()
    {
        constexpr  Vertex vertices[] =
        {
            vec3(-0.5f, 0.5f, 0.f),	vec3(1.f, 0.f, 0.f), vec2(0.f, 1.f), vec3(0.f, 0.f, 1.f),
            vec3(-0.5f, -0.5f, 0.f), vec3(0.f, 1.f, 0.f), vec2(0.f, 0.f), vec3(0.f, 0.f, 1.f),
            vec3(0.5f, -0.5f, 0.f), vec3(0.f, 0.f, 1.f), vec2(1.f, 0.f), vec3(0.f, 0.f, 1.f),
            vec3(0.5f, 0.5f, 0.f), vec3(1.f, 1.f, 0.f), vec2(1.f, 1.f), vec3(0.f, 0.f, 1.f)
        };
        constexpr unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

        constexpr GLuint indices[] =
        {
            0, 1, 2,
            0, 2, 3	
        };
        constexpr unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

        this->set(vertices, nrOfVertices, indices, nrOfIndices);
    }
};

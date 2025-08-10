#pragma once

#include "Classes/Vertex.h"

class Mesh
{
    // unsigned nrOfVertices;
    // unsigned nrOfIndices;
    vector<Vertex> vertices;
    vector<GLuint> indices;

    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    vec3 position;
    vec3 rotation;
    vec3 scale;

    mat4 ModelMatrix;

    void initVertexData(Vertex* vertexArray, const unsigned nrOfVertices, GLuint* indexArray, const unsigned nrOfIndices)
    {
        for (size_t i = 0; i < nrOfVertices; i++)
        {
            this->vertices.push_back(vertexArray[i]);
        }

        for (size_t i = 0; i < nrOfIndices; i++)
        {
            this->indices.push_back(indexArray[i]);
        }
    }

    void initVAO()
    {
        glCreateVertexArrays(1, &this->VAO);
        glBindVertexArray(this->VAO);
        
        glGenBuffers(1, &this->VBO);
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), this->vertices.data(), GL_STATIC_DRAW);
        
        glGenBuffers(1, &this->EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(Vertex), this->indices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, position));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, color));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, texCoord));
        glEnableVertexAttribArray(2);

        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, normal));
        glEnableVertexAttribArray(3);

        glBindVertexArray(0);
    }

    void updateModelMatrix()
    {
        this->position = vec3(0.f);
        this->rotation = vec3(0.f);
        this->scale = vec3(1.f);
    
        this->ModelMatrix = mat4(1.f);
        this->ModelMatrix = translate(this->ModelMatrix, this->position);
        this->ModelMatrix = rotate(this->ModelMatrix, radians(this->rotation.x), vec3(1.f, 0.f, 0.f));
        this->ModelMatrix = rotate(this->ModelMatrix, radians(this->rotation.y), vec3(0.f, 1.f, 0.f));
        this->ModelMatrix = rotate(this->ModelMatrix, radians(this->rotation.z), vec3(0.f, 0.f, 1.f));
        this->ModelMatrix = glm::scale(this->ModelMatrix, this->scale);
    }

    void updateUniforms(Shader* shader)
    {
        shader->setMat4fv(this->ModelMatrix, "ModelMatrix");
    }
    
public:
    Mesh(
        Vertex* vertexArray,
        const unsigned& nrOfVertices,
        GLuint* indexArray,
        const unsigned& nrOfIndices
    )
    {
        this->initVertexData(vertexArray, nrOfVertices, indexArray, nrOfIndices);
        this->initVAO();
        this->updateModelMatrix();
    }

    ~Mesh()
    {
        glDeleteVertexArrays(1, &this->VAO);
        glDeleteBuffers(1, &this->VBO);
        glDeleteBuffers(1, &this->EBO);
    }

    void update()
    {

    }

    void render(Shader* shader)
    {
        this->updateModelMatrix();
        this->updateUniforms(shader);

        shader->use();
        
        glBindVertexArray(this->VAO);
        
        glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
    }
};

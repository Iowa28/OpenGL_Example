#pragma once

#include "Classes/Vertex.h"

class Mesh
{
    unsigned nrOfVertices;
    unsigned nrOfIndices;

    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    vec3 position;
    vec3 rotation;
    vec3 scale;

    mat4 ModelMatrix;

    void initVAO(Vertex* vertexArray, const unsigned nrOfVertices, GLuint* indexArray, const unsigned nrOfIndices)
    {
        this->nrOfVertices = nrOfVertices;
        this->nrOfIndices = nrOfIndices;
        
        glCreateVertexArrays(1, &this->VAO);
        glBindVertexArray(this->VAO);
        
        glGenBuffers(1, &this->VBO);
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferData(GL_ARRAY_BUFFER, this->nrOfVertices * sizeof(Vertex), vertexArray, GL_STATIC_DRAW);
        
        glGenBuffers(1, &this->EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->nrOfIndices * sizeof(Vertex), indexArray, GL_STATIC_DRAW);

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
        this->ModelMatrix = mat4(1.f);
        this->ModelMatrix = translate(this->ModelMatrix, this->position);
        this->ModelMatrix = glm::rotate(this->ModelMatrix, radians(this->rotation.x), vec3(1.f, 0.f, 0.f));
        this->ModelMatrix = glm::rotate(this->ModelMatrix, radians(this->rotation.y), vec3(0.f, 1.f, 0.f));
        this->ModelMatrix = glm::rotate(this->ModelMatrix, radians(this->rotation.z), vec3(0.f, 0.f, 1.f));
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
        const unsigned& nrOfIndices,
        vec3 position = vec3(0.f),
        vec3 rotation = vec3(0.f),
        vec3 scale = vec3(1.f)
    )
    {
        this->position = position;
        this->rotation = rotation;
        this->scale = scale;
        
        this->initVAO(vertexArray, nrOfVertices, indexArray, nrOfIndices);
        this->updateModelMatrix();
    }

    // Mesh(
    //     Primitive* primitive,
    //     vec3 position = vec3(0.f),
    //     vec3 rotation = vec3(0.f),
    //     vec3 scale = vec3(1.f))
    // {
    //     this->position = position;
    //     this->rotation = rotation;
    //     this->scale = scale;
    //
    //     this->initVAO(primitive);
    //     this->updateModelMatrix();
    // }

    ~Mesh()
    {
        glDeleteVertexArrays(1, &this->VAO);
        glDeleteBuffers(1, &this->VBO);
        glDeleteBuffers(1, &this->EBO);
    }


    //Accessors

    //Modifiers
    
    void setPosition(const vec3 position)
    {
        this->position = position;
    }

    void setRotation(const vec3 rotation)
    {
        this->rotation = rotation;
    }

    void setScale(const vec3 setScale)
    {
        this->scale = setScale;
    }

    //Functions

    void move(const vec3 position)
    {
        this->position += position;
    }

    void rotate(const vec3 rotation)
    {
        this->rotation += rotation;
    }

    void scaleUp(const vec3 scale)
    {
        this->scale *= scale;
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
        glDrawElements(GL_TRIANGLES, this->nrOfIndices, GL_UNSIGNED_INT, 0);
    }
};

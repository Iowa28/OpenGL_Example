#pragma once

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>
#include <mat4x4.hpp>
#include <gtc/type_ptr.hpp>

using namespace glm;

class Material
{
private:
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    GLint diffuseTex;
    GLint specularTex;
    
public:
    Material(const vec3& ambient, const vec3& diffuse, const vec3& specular, GLint diffuseTex, GLint specularTex)
        : ambient(ambient),
          diffuse(diffuse),
          specular(specular),
          diffuseTex(diffuseTex),
          specularTex(specularTex)
    {
    }

    ~Material()
    {
    }

    void sendToShader(Shader& shader)
    {
        shader.setVec3f(this->ambient, "material.ambient");
        shader.setVec3f(this->diffuse, "material.diffuse");
        shader.setVec3f(this->specular, "material.specular");
        shader.set1i(this->diffuseTex, "material.diffuseTex");
        shader.set1i(this->specularTex, "material.specularTex");
    }
};

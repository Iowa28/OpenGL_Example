#pragma once

using namespace std;

class Shader
{
    GLuint id;

    string loadShaderSource(const char* fileName)
    {
        string temp = "";
        string src = "";
        ifstream in_file;

        in_file.open(fileName);

        if (in_file.is_open())
        {
            while (getline(in_file, temp))
            {
                src += temp + "\n";
            }
        }
        else
        {
            cout << "ERROR::SHADER::COULD_NOT_OPEN_FILE: " << fileName << "\n";
        }

        in_file.close();
        return src;
    }

    GLuint loadShader(GLenum type, const char* fileName)
    {
        char infoLog[512];
        GLint success;

        const GLuint shader = glCreateShader(type);
        const string str_src = this->loadShaderSource(fileName);
        const GLchar* src = str_src.c_str();
        glShaderSource(shader, 1, &src, NULL);
        glCompileShader(shader);

        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            cout << "ERROR::SHADER::COULD_NOT_COMPILE_SHADER: " << fileName << "\n";
            cout << infoLog << "\n";
        }

        return shader;
    }

    void linkProgram(GLuint vertexShader, GLuint geometryShader, GLuint fragmentShader)
    {
        char infoLog[512];
        GLint success;

        this->id = glCreateProgram();

        glAttachShader(this->id, vertexShader);

        if (geometryShader)
        {
            glAttachShader(this->id, geometryShader);
        }

        glAttachShader(this->id, fragmentShader);

        glLinkProgram(this->id);

        glGetProgramiv(this->id, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(this->id, 512, nullptr, infoLog);
            cout << "ERROR::SHADER::COULD_NOT_LINK_PROGRAM" << "\n";
            cout << infoLog << "\n";
        }

        glUseProgram(0);
    }

public:

    Shader(const char* vertexFile, const char* fragmentFile, const char* geometryFile = "")
    {
        GLuint geometryShader = 0;

        const GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vertexFile);
        const GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentFile);

        if (geometryFile != "")
        {
            geometryShader = loadShader(GL_GEOMETRY_SHADER, geometryFile);
        }

        this->linkProgram(vertexShader, geometryShader, fragmentShader);

        //End
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteShader(geometryShader);
    }

    ~Shader()
    {
        glDeleteProgram(this->id);
    }
    
    void use()
    {
        glUseProgram(this->id);
    }

    void unuse()
    {
        glUseProgram(0);
    }

    void set1i(GLint value, const GLchar* name)
    {
        glUniform1i(glGetUniformLocation(this->id, name), value);
    }

    void set1f(GLfloat value, const GLchar* name)
    {
        glUniform1f(glGetUniformLocation(this->id, name), value);
    }

    void setVec2f(fvec2 value, const GLchar* name)
    {
        glUniform2fv(glGetUniformLocation(this->id, name), 1, value_ptr(value));
    }

    void setVec3f(fvec3 value, const GLchar* name)
    {
        glUniform3fv(glGetUniformLocation(this->id, name), 1, value_ptr(value));
    }

    void setVec4f(fvec4 value, const GLchar* name)
    {
        glUniform4fv(glGetUniformLocation(this->id, name), 1, value_ptr(value));
    }

    void setMat3fv(mat3 value, const GLchar* name, GLboolean transpose = GL_FALSE)
    {
        glUniformMatrix3fv(glGetUniformLocation(this->id, name), 1, transpose, value_ptr(value));
    }

    void setMat4fv(mat4 value, const GLchar* name, GLboolean transpose = GL_FALSE)
    {
        glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, transpose, value_ptr(value));
    }
};

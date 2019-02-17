#pragma once
#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "DecartVector.hpp"
#include "Matrices.hpp"
#include "Texture.hpp"

namespace am
{

class Uniform
{
public:
    Uniform()
    {
        glGetIntegerv(GL_CURRENT_PROGRAM, &shaderId);
    }
    Uniform(sf::Shader* shader)
    {
        sf::Shader::bind(shader);
        glGetIntegerv(GL_CURRENT_PROGRAM, &shaderId);
        sf::Shader::bind(NULL);
    }
    void SetUniform(const char* name, int val)
    {
        GLint location = glGetUniformLocation(shaderId, name);
        glUniform1i(location, val);
    }
    void SetUniform(const char* name, float val)
    {
        GLint location = glGetUniformLocation(shaderId, name);
        glUniform1f(location, val);
    }
    void SetUniform(const char* name, am::Vector2f val)
    {
        GLint location = glGetUniformLocation(shaderId, name);
        glUniform2f(location, val.x, val.y);
    }
    void SetUniform(const char* name, am::Vector3f val)
    {
        GLint location = glGetUniformLocation(shaderId, name);
        glUniform3f(location, val.x, val.y, val.z);
    }
    void SetUniform(const char* name, am::Vector4f val)
    {
        GLint location = glGetUniformLocation(shaderId, name);
        glUniform4f(location, val.x, val.y, val.z, val.w);
    }
    void SetUniform(const char* name, am::Matrix3x3f val)
    {
        GLint location = glGetUniformLocation(shaderId, name);
        glUniformMatrix3fv(location, 1, false, (float*)&val);
    }
    void SetUniform(const char* name, am::Matrix4x4f val)
    {
        GLint location = glGetUniformLocation(shaderId, name);
        glUniformMatrix4fv(location, 1, false, (float*)&val);
    }
    void BindTexture(const char *name, am::Texture* texture, int index)
    {
        GLint location = glGetUniformLocation(shaderId, name);
        glActiveTexture(GL_TEXTURE0+index);
        glUniform1i(location, index);
        glBindTexture(GL_TEXTURE_2D, texture->texId);
    }
    void UnbindTexture(int index)
    {
        glActiveTexture(GL_TEXTURE0+index);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    void BindCubeTexture(const char *name, am::CubeTexture* texture, int index)
    {
        GLint location = glGetUniformLocation(shaderId, name);
        glActiveTexture(GL_TEXTURE0+index);
        glUniform1i(location, index);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture->texId);
    }
    void UnbindCubeTexture(int index)
    {
        glActiveTexture(GL_TEXTURE0+index);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }

private:
    GLint shaderId;
};
};

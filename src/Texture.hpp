#pragma once
#define GL_GLEXT_PROTOTYPES
#define _SECURE_SCL 0
#include "SFML/OpenGL.hpp"
#include "Stb_image.h"
#include <string>
#include <vector>
#include <stdio.h>

namespace am
{
class Texture
{
public:
private:
    Texture(std::string filename)
    {
        glGenTextures(1, &texId);
        if(stbi_is_hdr(filename.data()))
            LoadHDRFromFile(filename);
        else
            LoadNotHDRFromFile(filename);
    }
    Texture(float r, float g, float b, float a = 1.f)
    {
        glGenTextures(1, &texId);
        std::vector<float> v;
        v.push_back(r);
        v.push_back(g);
        v.push_back(b);
        v.push_back(a);
        CreateFromColor(v.data());
    }
    Texture(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255)
    {
        glGenTextures(1, &texId);
        std::vector<unsigned char> v;
        v.push_back(r);
        v.push_back(g);
        v.push_back(b);
        v.push_back(a);
        CreateFromColor(v.data());
    }
    void CreateFromColor(const float* color)
    {
        glBindTexture(GL_TEXTURE_2D, texId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 1, 1, 0, GL_RGBA, GL_FLOAT, color);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    void CreateFromColor(const unsigned char* color)
    {
        glBindTexture(GL_TEXTURE_2D, texId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, color);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    void LoadNotHDRFromFile(std::string filename)
    {
        FILE* file = fopen(filename.data(), "r");
        int x, y, comp;
        unsigned char * data = stbi_load_from_file(file, &x, &y, &comp, 0);
        fclose(file);
        GLenum format;
        if(comp==3) format = GL_RGB;
        else format = GL_RGBA;
        LoadFromMemory(data, x, y, format);
        stbi_image_free(data);
    }
    void LoadHDRFromFile(std::string filename)
    {
        FILE* file = fopen(filename.data(), "r");
        int x, y, comp;
        float* data = stbi_loadf_from_file(file, &x, &y, &comp, 0);
        fclose(file);
        GLenum format;
        if(comp==3) format = GL_RGB;
        else format = GL_RGBA;
        LoadHDRFromMemory(data, x, y, format);
        stbi_image_free(data);
    }
    void LoadHDRFromMemory(float *data, int x, int y, GLenum format)
    {
        glBindTexture(GL_TEXTURE_2D, texId);
        gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGBA32F, x, y, format, GL_FLOAT, data );
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, x, y, 0, format, GL_FLOAT, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    void LoadFromMemory(unsigned char* data, int x, int y, GLenum format)
    {
        glBindTexture(GL_TEXTURE_2D, texId);
        gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGBA, x, y, format, GL_UNSIGNED_BYTE, data );
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, format, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    ~Texture()
    {
        glDeleteTextures(1, &texId);
    }
    GLuint texId;
friend class Renderer;
friend class Material;
friend class Uniform;
};

class CubeTexture
{
private:
    CubeTexture(std::vector<std::string> filename)
    {
        if(filename.size()>0)
        {
            while(filename.size()<6)
                filename.push_back(filename[filename.size()-1]);
            glGenTextures(1, &texId);
            glBindTexture(GL_TEXTURE_CUBE_MAP, texId);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            for(int i = 0; i<6; i++)
            {
                if(stbi_is_hdr(filename[i].data()))
                    LoadHDRFromFile(filename[i], i);
                else
                    LoadNotHDRFromFile(filename[i], i);
            }
            glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        }
    }
    ~CubeTexture()
    {
        glDeleteTextures(1, &texId);
    }
    void LoadHDRFromFile(std::string filename, int index)
    {
        float* data;
        int x, y, comp;
        GLenum format;
        FILE* file = fopen(filename.data(), "r");
        data = stbi_loadf_from_file(file, &x, &y, &comp, 0);
        fclose(file);
        if(comp==3)
            format = GL_RGB;
        else
            format = GL_RGBA;
        //gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_X+index, GL_RGBA32F, x, y, format, GL_FLOAT, data );
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+index, 0, GL_RGBA32F, x, y, 0, format, GL_FLOAT, data);
        stbi_image_free(data);
    }
    void LoadNotHDRFromFile(std::string filename, int index)
    {
        unsigned char* data;
        int x, y, comp;
        GLenum format;
        FILE* file = fopen(filename.data(), "r");
        data = stbi_load_from_file(file, &x, &y, &comp, 0);
        fclose(file);
        if(comp==3)
            format = GL_RGB;
        else
            format = GL_RGBA;
        //gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_X+index, GL_RGBA, x, y, format, GL_UNSIGNED_BYTE, data );
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+index, 0, GL_RGBA, x, y, 0, format, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }

    GLuint texId;
friend class Renderer;
friend class Material;
friend class Uniform;
};

};

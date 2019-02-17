#pragma once
#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "Texture.hpp"
#include "Uniform.hpp"

namespace am
{

class Material
{
public:

private:
    Material(int mode = 2)
    {
        this->mode = mode;
    }
    ~Material()
    {
        textures.clear();
        cubemaps.clear();
    }
    int GetTexNumber()
    {
        return textures.size()+cubemaps.size();
    }

    void AddTexture(am::Texture* texture, std::string nameInShader)
    {
        if(texture!=NULL)
            textures.push_back(TextureWithInfo(texture, nameInShader));
    }
    void AddCubemap(am::CubeTexture* cubemap, std::string nameInShader)
    {
        if(cubemap!=NULL)
            cubemaps.push_back(TextureCubeWithInfo(cubemap, nameInShader));
    }
    void BindTextures(Uniform* unf)
    {
        for(int i = 0; i < textures.size(); i++)
        {
            unf->BindTexture(textures[i].name.data(), textures[i].tex, i);
        }
        for(int i = 0; i < cubemaps.size(); i++)
        {
            unf->BindCubeTexture(cubemaps[i].name.data(), cubemaps[i].tex, i+textures.size());
        }
    }
    void UnbindTextures(Uniform* unf)
    {
        for(int i = 0; i < textures.size(); i++)
        {
            unf->UnbindTexture(i);
        }
        for(int i = 0; i < cubemaps.size(); i++)
        {
            unf->UnbindCubeTexture(i+textures.size());
        }
    }
    struct TextureWithInfo
    {
    public:
        TextureWithInfo(am::Texture* texture, std::string nameInShader)
        {
            this->tex = texture;
            this->name = nameInShader;
        }
        am::Texture* tex;
        std::string name;
    };
    struct TextureCubeWithInfo
    {
    public:
        TextureCubeWithInfo(am::CubeTexture* cubemap, std::string nameInShader)
        {
            this->tex = cubemap;
            this->name = nameInShader;
        }
        am::CubeTexture* tex;
        std::string name;
    };
    std::vector<TextureWithInfo> textures;
    std::vector<TextureCubeWithInfo> cubemaps;
    int mode;
friend class Renderer;
friend class Entity;
};
};

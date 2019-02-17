#define GL_GLEXT_PROTOTYPES
#define _SECURE_SCL 0
#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/OpenGL.hpp"
#include "GL/glu.h"
#include "DecartVector.hpp"
#include "Matrices.hpp"
#include "Coords3f.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "Material.hpp"
#include "Uniform.hpp"

namespace am
{

struct Camera
{
public:

    float fov;
    float nearP, farP;
    Coords3f coords;
    Matrix4x4f projMatrix;
};

class Entity
{
public:
    Coords3f coords;
private:
    Entity(Mesh* mesh, Coords3f coords, Material* material, CubeTexture* envmap = NULL)
    {
        this->mesh = mesh;
        this->coords = coords;
        this->material = material;
        this->envmap = envmap;
    }
    ~Entity()
    {
        material = NULL;
        mesh = NULL;
    }
    void Render(Uniform* unf)
    {
        Matrix3x3f m = coords.GetMatrix();
        m.Transpose();
        am::Vector3f pos = coords.pos;
        unf->SetUniform("mm", m);
        unf->SetUniform("mp", pos);
        if(envmap!=NULL)
            unf->BindCubeTexture("envmap", envmap, material->GetTexNumber());
        mesh->DrawArrays();
        if(envmap!=NULL)
            unf->UnbindCubeTexture(material->GetTexNumber());
    }
    friend class Renderer;
private:
    int size;
    Material* material;
public:
    Mesh* mesh;
    CubeTexture* envmap;
};

class Renderer
{
public:
    am::Matrix3x3f envModelMatrix;
    void SetExp(float f)
    {
        shader->setParameter("exposure", f);
    }
    void SetCurrentTime(float f)
    {
        shader->setParameter("currentTime", f);
    }

    Renderer(sf::RenderTarget* window, std::string vertshader, std::string fragshader)
    {
        this->window = window;
        shader = new sf::Shader();
        shader->loadFromFile(vertshader, fragshader);
        envModelMatrix.SetIdentity();
        SetView();
    }
    ~Renderer()
    {
        for(auto i = mesh.begin(); i!=mesh.end(); i++)
        {
            delete(i->second);
        }
        for(auto i = texture.begin(); i!=texture.end(); i++)
        {
            delete(i->second);
        }
        for(auto i = texCube.begin(); i!=texCube.end(); i++)
        {
            delete(i->second);
        }
        for(auto i = material.begin(); i!=material.end(); i++)
        {
            delete(i->second);
        }
        for(auto i = entity.begin(); i!=entity.end(); i++)
        {
            delete(i->second);
        }
    }
    void SetSkyExp(float k)
    {
        shader->setParameter("exp", k);
    }

///Работа со сценой: загрузка текстур, мешей, материалов и объектов
    Mesh* LoadMesh(std::string name, std::string filename)
    {
        if(name.empty())
        {
            name = "#noname-";
            name.append(std::to_string(mesh.size()));
        }
        if(mesh.find(name)==mesh.end())
        {
            Mesh* m = new UsualMesh(filename);
            mesh[name] = m;
            return m;
        }
        else return mesh[name];
    }
    Mesh* LoadMesh(std::string name, std::vector<am::Vector3f> vertexData, std::vector<am::Vector3f> normalData,
                   std::vector<am::Vector2f> texCoordData, std::vector<unsigned int> indexData)
    {
        if(name.empty())
        {
            name = "#noname-";
            name.append(std::to_string(mesh.size()));
        }
        if(mesh.find(name)==mesh.end())
        {
            Mesh* m = new UsualMesh(vertexData, normalData, texCoordData, indexData);
            mesh[name] = m;
            return m;
        }
        else return mesh[name];
    }
    am::Texture* LoadTexture(std::string name, std::string filename)
    {
        if(name.empty())
        {
            name = "#noname-";
            name.append(std::to_string(texture.size()));
        }
        if(texture.find(name)==texture.end())
        {
            am::Texture* tex = new am::Texture(filename);
            texture[name] = tex;
            return tex;
        }
        else return texture[name];
    }
    am::Texture* LoadTexture(std::string name, sf::Color color)
    {
        if(name.empty())
        {
            name = "#noname-";
            name.append(std::to_string(texture.size()));
        }
        if(texture.find(name)==texture.end())
        {
            am::Texture* tex = new am::Texture((unsigned char)color.r, color.g, color.b, color.a);
            texture[name] = tex;
            return tex;
        }
        else return texture[name];
    }
    am::Texture* GetTexture(std::string name)
    {
        if(texture.find(name)!=texture.end())
        {
            return texture[name];
        }
        else return NULL;
    }
    am::Texture* UpdateTexture(std::string name, sf::Color color)
    {
        if(texture.find(name)!=texture.end())
        {
            texture[name]->CreateFromColor((unsigned char*)&color);
        }
        else return NULL;
    }
    am::CubeTexture* LoadCubeTexture(std::string name, std::vector<std::string> filename)
    {
        if(name.empty())
        {
            name = "#noname-";
            name.append(std::to_string(texCube.size()));
        }
        if(texCube.find(name)==texCube.end())
        {
            am::CubeTexture* tex = new am::CubeTexture(filename);
            texCube[name] = tex;
            return tex;
        }
        else return texCube[name];
    }
    am::CubeTexture* GetCubeTexture(std::string name)
    {
        if(texCube.find(name)!=texCube.end())
        {
            return texCube[name];
        }
        else return NULL;
    }
    am::Material* LoadMaterial(std::string name, am::Texture* albedo = NULL, am::Texture* microsurface = NULL,
                           am::Texture* reflectivity = NULL, am::Texture* fresnel = NULL,
                           am::Texture* normalMap = NULL)
    {
        if(name.empty())
        {
            name = "#noname-";
            name.append(std::to_string(material.size()));
        }
        if(material.find(name)==material.end())
        {
            Material* mat = new Material(2);
            mat->AddTexture(albedo, "albedo");
            mat->AddTexture(microsurface, "microsurface");
            mat->AddTexture(reflectivity, "reflectivity");
            mat->AddTexture(fresnel, "fresnel");
            mat->AddTexture(normalMap, "normalMap");
            material[name] = mat;
            return mat;
        }
        else return material[name];
    }
    am::Material* LoadMaterial(std::string name, am::CubeTexture* sky)
    {
        if(name.empty())
        {
            name = "#noname-";
            name.append(std::to_string(material.size()));
        }
        if(material.find(name)==material.end())
        {
            Material* mat = new Material(3);
            mat->AddCubemap(sky, std::string("sky"));
            material[name] = mat;
            return mat;
        }
        else return material[name];
    }
    am::Material* LoadMaterial(std::string name, int mode)
    {
        if(name.empty())
        {
            name = "#noname-";
            name.append(std::to_string(material.size()));
        }
        if(material.find(name)==material.end())
        {
            am::Material* mat = new am::Material(mode);
            material[name] = mat;
            return mat;
        }
        else return material[name];
    }
    void AddTextureToMaterial(std::string matName, std::string texName, std::string texNameInShader)
    {
        if(material.find(matName)!=material.end())
        {
            if(texture.find(texName)!=texture.end())
            {
                Material* mat = material[matName];
                mat->AddTexture(texture[texName], texNameInShader);
            }
        }
    }
    am::Material* GetMaterial(std::string name)
    {
        if(material.find(name)!=material.end())
            return material[name];
        else return NULL;
    }
    am::Entity* LoadEntity(std::string name, Mesh* mesh, Coords3f coords, Material* material, CubeTexture* envmap = NULL)
    {
        if(name.empty())
        {
            name = "#noname-";
            name.append(std::to_string(entity.size()));
        }
        if(entity.find(name)==entity.end())
        {
            Entity* ent = new Entity(mesh, coords, material, envmap);
            entity[name] = ent;
            materialMap[material].push_back(ent);
            return ent;
        }
        else return entity[name];
    }
    am::Entity* CreateSky(am::Material* material)
    {
        if(entity.find("sky")==entity.end())
        {
            std::vector<am::Vector3f> vert;
            std::vector<GLubyte> ind;
            vert.push_back(am::Vector3f(-1, -1, -1));
            vert.push_back(am::Vector3f(-1, -1, 1));
            vert.push_back(am::Vector3f(-1, 1, 1));
            vert.push_back(am::Vector3f(-1, 1, -1));
            vert.push_back(am::Vector3f(1, 1, -1));
            vert.push_back(am::Vector3f(1, -1, -1));
            vert.push_back(am::Vector3f(1, -1, 1));
            vert.push_back(am::Vector3f(1, 1, 1));
            ind.push_back(0);
            ind.push_back(1);
            ind.push_back(2);
            ind.push_back(3);
            ind.push_back(4);
            ind.push_back(5);
            ind.push_back(6);
            ind.push_back(7);
            ind.push_back(3);
            ind.push_back(4);
            ind.push_back(7);
            ind.push_back(2);
            ind.push_back(5);
            ind.push_back(0);
            ind.push_back(1);
            ind.push_back(6);
            ind.push_back(6);
            ind.push_back(7);
            ind.push_back(2);
            ind.push_back(1);
            ind.push_back(5);
            ind.push_back(4);
            ind.push_back(3);
            ind.push_back(0);
            am::Mesh* mesh = new am::SimpleTinyMesh(vert, ind, (GLenum)GL_QUADS);
            am::Coords3f coords;
            coords.LocalScale(am::Vector3f(camera.farP, camera.farP, camera.farP));
            am::Entity* ent = new Entity(mesh, coords, material);
            entity["sky"] = ent;
            materialMap[material].push_back(ent);
            return ent;
        }
        else return entity["sky"];
    }

    void RemoveMaterial(std::string name)
    {
        if(material.find(name)!=material.end())
        {
            am::Material* mat = material[name];
            materialMap.erase(mat);
            material.erase(name);
            delete(mat);
        }
    }
    void RemoveCubeTexture(std::string name)
    {
        if(texCube.find(name)!=texCube.end())
        {
            delete(texCube[name]);
            texCube.erase(name);
        }
    }
    void RemoveTexture(std::string name)
    {
        if(texture.find(name)!=texture.end())
        {
            delete(texture[name]);
            texture.erase(name);
        }
    }
    void RemoveEntity(std::string name)
    {
        if(entity.find(name)!=entity.end())
        {
            Entity* ent = entity[name];
            std::vector<Entity*> it = materialMap[ent->material];
            for(auto i = it.begin(); i!=it.end(); i++)
            {
                if(*i==ent)
                {
                    it.erase(i);
                    break;
                }
            }
            entity.erase(name);
            delete(ent);
        }
    }
    void RemoveMesh(std::string name)
    {
        if(mesh.find(name)!=mesh.end())
        {
            delete(mesh[name]);
            mesh.erase(name);
        }
    }

///---
///Основная функция - рендеринг кадра
    void RenderFrame()
    {
        SetView();
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //glEnable(GL_BLEND);
        // Сглаживание точек
        //glEnable(GL_POINT_SMOOTH);
        //glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
        // Сглаживание линий
        //glEnable(GL_LINE_SMOOTH);
        //glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
        // Сглаживание полигонов
        //glEnable(GL_POLYGON_SMOOTH);
        //glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
        /*glEnable(GL_DEPTH_TEST);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_TEXTURE_CUBE_MAP);
        glDepthFunc(GL_LESS);
        glClear(GL_DEPTH_BUFFER_BIT);
        glClearDepth(1.f);
        GLuint textureId;
        sf::Vector2u v = window->getSize();
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, v.x, v.y, 0,
                     GL_RGBA, GL_FLOAT, 0);
        glBindTexture(GL_TEXTURE_2D, 0);

        // create a renderbuffer object to store depth info
        GLuint rboId;
        glGenRenderbuffers(1, &rboId);
        glBindRenderbuffer(GL_RENDERBUFFER, rboId);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
                              v.x, v.y);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        // create a framebuffer object
        GLuint fboId;
        glGenFramebuffers(1, &fboId);
        glBindFramebuffer(GL_FRAMEBUFFER, fboId);

        // attach the texture to FBO color attachment point
        glFramebufferTexture2D(GL_FRAMEBUFFER,        // 1. fbo target: GL_FRAMEBUFFER
                               GL_COLOR_ATTACHMENT0,  // 2. attachment point
                               GL_TEXTURE_2D,         // 3. tex target: GL_TEXTURE_2D
                               textureId,             // 4. tex ID
                               0);                    // 5. mipmap level: 0(base)

        // attach the renderbuffer to depth attachment point
        glFramebufferRenderbuffer(GL_FRAMEBUFFER,      // 1. fbo target: GL_FRAMEBUFFER
                                  GL_DEPTH_ATTACHMENT, // 2. attachment point
                                  GL_RENDERBUFFER,     // 3. rbo target: GL_RENDERBUFFER
                                  rboId);              // 4. rbo ID

        // check FBO status
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        */
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_TEXTURE_CUBE_MAP);
        glDepthFunc(GL_LESS);
        glClear(GL_DEPTH_BUFFER_BIT);
        glClearDepth(1.f);
        Matrix3x3f m = camera.coords.GetMatrix();
        am::Vector3f pos = camera.coords.pos;
        // switch back to window-system-provided framebuffer
        /*glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDeleteRenderbuffers(1, &rboId);
        glDeleteFramebuffers(1, &fboId);*/
        sf::Shader::bind(shader);
        am::Uniform unf;
        unf.SetUniform("skymm", envModelMatrix);
        unf.SetUniform("mpro", camera.projMatrix);
        unf.SetUniform("vm", m);
        unf.SetUniform("vp", pos);
        for(auto it = materialMap.begin(); it!=materialMap.end(); it++)
        {
            Material* mat = it->first;
            unf.SetUniform("mode", mat->mode);
            mat->BindTextures(&unf);
            for(int i = 0; i < it->second.size(); i++)
            {
                (it->second)[i]->Render(&unf);
            }
            mat->UnbindTextures(&unf);
        }
        //glDeleteTextures(1, &textureId);
        sf::Shader::bind(NULL);
    }
///Вспомогательные функции
    void SetView()
    {
        camera.projMatrix = am::Transpose(BuildProjectionMatrix(30.f, 1.f*window->getSize().x/window->getSize().y, 1.f, 10000.f));
    }

    Matrix4x4f BuildProjectionMatrix(float fov, float ratio, float nearP, float farP)
    {
        Matrix4x4f m;
        m.SetIdentity();
        float f = 1.f/tan(fov*M_PI/360.f);
        camera.fov = fov;
        camera.farP = farP;
        camera.nearP = nearP;
        m[0][0] = f/ratio;
        m[1][1] = f;
        m[2][2] = (farP+nearP)/(farP-nearP);
        m[2][3] = -1.0f;
        m[3][2] = 2.f*farP*nearP/(nearP-farP);
        m[3][3] = 0.f;
        return m;
    }
    int GetEntityCount()
    {
        return entity.size();
    }
    Entity* GetEntity(int i)
    {
        if(i<entity.size())
        {
            auto it = entity.begin();
            for(int k = 0; k < i; k ++)
                it++;
            return it->second;
        }
        else return NULL;
    }
    Entity* GetEntity(std::string name)
    {
        if(entity.find(name)!=entity.end())
        {
            return entity[name];
        }
        else return NULL;
    }

    Camera camera;
private:

    sf::RenderTarget* window;
    sf::Shader* shader;
    std::map<std::string, Entity*> entity;
    std::map<std::string, am::Texture*> texture;
    std::map<std::string, am::CubeTexture*> texCube;
    std::map<Material*, std::vector<Entity*> > materialMap;
    std::map<std::string, Mesh*> mesh;
    std::map<std::string, Material*> material;
};
};

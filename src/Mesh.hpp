#pragma once
#define GL_GLEXT_PROTOTYPES
#define _SECURE_SCL 0
#include "SFML/OpenGL.hpp"
#include <map>
#include <string>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <limits>
#include "DecartVector.hpp"
#include "Matrices.hpp"

namespace am
{
class Renderer;
class Entity;

enum MeshEnum
{
    VERTEX_ONLY,
    NORMAL_VERTEX,
    TEXCOORD_VERTEX,
    ALL
};

struct IndexedVertex
{
public:
    IndexedVertex(int vert, int tex, int norm)
    {
        this->vert = vert;
        this->tex = tex;
        this->norm = norm;
    }
    void Update(int vert, int tex, int norm)
    {
        this->vert = vert;
        this->tex = tex;
        this->norm = norm;
    }

    int vert, tex, norm;
};
bool operator < (const IndexedVertex &v1, const IndexedVertex &v2)
{
    if(v1.vert < v2.vert) return true;
    if(v1.vert > v2.vert) return false;
    if(v1.tex < v2.tex) return true;
    if(v1.tex > v2.tex) return false;
    if(v1.norm < v2.norm) return true;
    else return false;
}

class Mesh
{
public:
    am::Vector3f* GetVertexData()
    {
        return vertexData.data();
    }
    int GetSize()
    {
        return vertexData.size();
    }
    GLuint GetVertexBuffer()
    {
        return vertexBuffer;
    }
    virtual void DrawArrays() = 0;
    virtual bool NewLoadFromFile(std::string filename) = 0;
    virtual void SaveToFile(std::string filename, MeshEnum en) = 0;
    void TransformVertices(Matrix3x3f mv)
    {
        for(int i = 0; i < vertexData.size(); i++)
        {
            vertexData[i] = mv*vertexData[i];
        }
        ReloadBuffers();
    }
    void AlignVertices()
    {
        float r = std::numeric_limits<float>::max();
        am::Vector3f min(r, r, r);
        r = std::numeric_limits<float>::min();
        am::Vector3f max(r, r, r);
        for(int i = 0; i < vertexData.size(); i++)
        {
            if(vertexData[i].x>max.x) max.x = vertexData[i].x;
            if(vertexData[i].y>max.y) max.y = vertexData[i].y;
            if(vertexData[i].z>max.z) max.z = vertexData[i].z;
            if(vertexData[i].x<min.x) min.x = vertexData[i].x;
            if(vertexData[i].y<min.y) min.y = vertexData[i].y;
            if(vertexData[i].z<min.z) min.z = vertexData[i].z;
        }
        am::Vector3f centre = -0.5f*(min+max);
        for(int i = 0; i < vertexData.size(); i++)
        {
            vertexData[i]+=centre;
        }
        ReloadBuffers();
    }
protected:
    GLuint vertexBuffer, vaobuffer;
    virtual void LoadBuffers() = 0;
    virtual void DeleteBuffers() = 0;
    void ReloadBuffers()
    {
        DeleteBuffers();
        LoadBuffers();
    }
public:
    std::vector<am::Vector3f> vertexData;
    friend class Renderer;
    friend class Application;
};

// Simple Mesh
class SimpleTinyMesh:public Mesh
{
public:
private:
    SimpleTinyMesh(std::vector<am::Vector3f> vertexData, std::vector<GLubyte> indexData, GLenum mode = GL_TRIANGLES)
    {
        this->indexData = indexData;
        this->vertexData = vertexData;
        this->mode = mode;
        LoadBuffers();
    }
    ~SimpleTinyMesh()
    {
        DeleteBuffers();
    }

    void LoadBuffers()
    {
        glGenVertexArrays(1, &vaobuffer);
        glBindVertexArray(vaobuffer);
        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*vertexData.size(), (float*)vertexData.data(), GL_DYNAMIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glGenBuffers(1, &indexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte)*indexData.size(), indexData.data(), GL_DYNAMIC_DRAW);
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    void DeleteBuffers()
    {
        glDeleteBuffers(1, &indexBuffer);
        glDeleteBuffers(1, &vertexBuffer);
        glDeleteVertexArrays(1, &vaobuffer);
    }
    void DrawArrays()
    {
        glBindVertexArray(vaobuffer);
        glDrawElements(mode, indexData.size(), GL_UNSIGNED_BYTE, NULL);
        glBindVertexArray(0);
    }
    bool NewLoadFromFile(std::string filename)
    {
        return false;
    }
    void SaveToFile(std::string filename, MeshEnum en)
    {

    }

    std::vector<GLubyte> indexData;
    GLuint indexBuffer;
    GLenum mode;
friend class am::Renderer;
friend class Application;
};

// Usual Mesh
class UsualMesh:public Mesh
{
public:
    GLuint GetNormalBuffer()
    {
        return normalBuffer;
    }
    GLuint GetTexCoordBuffer()
    {
        return texCoordBuffer;
    }
    am::Vector3f* GetNormalData()
    {
        return normalData.data();
    }
    am::Vector2f* GetTexCoordData()
    {
        return texCoordData.data();
    }
    am::Vector3f* GetTangData()
    {
        return tangData.data();
    }
    GLuint GetIndexBuffer()
    {
        return indexBuffer;
    }
    void DrawArrays()
    {
        glBindVertexArray(vaobuffer);
        glDrawElements(GL_TRIANGLES, indexData.size(), GL_UNSIGNED_INT, NULL);
        glBindVertexArray(0);
    }
    void ClearIdentityVertex()
    {
        std::map<int, int> num;
        std::vector<int> rep;
        std::map<am::Vector3f, int> map;
        for(int i = 0; i < vertexData.size(); i++)
        {
            if(map.find(vertexData[i])==map.end())
            {
                map[vertexData[i]] = i;
            }
            rep.push_back(map[vertexData[i]]);
        }
        std::vector<am::Vector3f> norm = normalData;
        std::vector<am::Vector2f> tex = texCoordData;
        normalData.clear();
        vertexData.clear();
        texCoordData.clear();
        int h = 0;
        for(auto it = map.begin(); it!=map.end(); it++)
        {
            vertexData.push_back(it->first);
            int k = it->second;
            texCoordData.push_back(tex[k]);
            normalData.push_back(norm[k]);
            num[k] = h;
            h++;
        }
        for(int i = 0; i < indexData.size(); i++)
        {
            indexData[i] = num[rep[indexData[i]]];
        }
        ReloadBuffers();
    }
    void SaveToFile(std::string filename, MeshEnum en)
    {
        std::ofstream file;
        file.open(filename);
        switch(en)
        {
        case ALL:
            for(int i = 0; i < this->vertexData.size(); i++)
            {
                file << "v " << vertexData[i].x << " " << vertexData[i].y << " " << vertexData[i].z <<"\n";
                file << "vt " << texCoordData[i].x << " " << texCoordData[i].y <<"\n";
                file << "vn " << normalData[i].x << " " << normalData[i].y << " " << normalData[i].z <<"\n";
            }
            for(int i = 0; i < indexData.size()/3; i++)
            {
                int l = i*3;
                file << "f " << indexData[l]+1 << "/" << indexData[l]+1 << "/" << indexData[l]+1;
                file << " " << indexData[l+1]+1 << "/" << indexData[l+1]+1 << "/" << indexData[l+1]+1;
                file << " " << indexData[l+2]+1 << "/" << indexData[l+2]+1 << "/" << indexData[l+2]+1 << "\n";
            }
            break;
        case TEXCOORD_VERTEX:
            for(int i = 0; i < this->vertexData.size(); i++)
            {
                file << "v " << vertexData[i].x << " " << vertexData[i].y << " " << vertexData[i].z <<"\n";
                file << "vt " << texCoordData[i].x << " " << texCoordData[i].y <<"\n";
            }
            for(int i = 0; i < indexData.size()/3; i++)
            {
                int l = i*3;
                file << "f " << indexData[l]+1 << "/" << indexData[l]+1;
                file << " " << indexData[l+1]+1 << "/" << indexData[l+1]+1;
                file << " " << indexData[l+2]+1 << "/" << indexData[l+2]+1 << "\n";
            }
            break;
        case NORMAL_VERTEX:
            for(int i = 0; i < this->vertexData.size(); i++)
            {
                file << "v " << vertexData[i].x << " " << vertexData[i].y << " " << vertexData[i].z <<"\n";
                file << "vn " << normalData[i].x << " " << normalData[i].y << " " << normalData[i].z <<"\n";
            }
            for(int i = 0; i < indexData.size()/3; i++)
            {
                int l = i*3;
                file << "f " << indexData[l]+1 << "/" << indexData[l]+1;
                file << " " << indexData[l+1]+1 << "/" << indexData[l+1]+1;
                file << " " << indexData[l+2]+1 << "/" << indexData[l+2]+1 << "\n";
            }
            break;
        case VERTEX_ONLY:
                for(int i = 0; i < this->vertexData.size(); i++)
                {
                    file << "v " << vertexData[i].x << " " << vertexData[i].y << " " << vertexData[i].z <<"\n";
                }
                for(int i = 0; i < indexData.size()/3; i++)
                {
                    int l = i*3;
                    file << "f " << indexData[l]+1;
                    file << " " << indexData[l+1]+1;
                    file << " " << indexData[l+2]+1 << "\n";
                }
                break;
        }
        file.close();
    }
private:
    GLuint normalBuffer, texCoordBuffer, indexBuffer, tangBuffer;
    UsualMesh(std::vector<am::Vector3f> vertexData, std::vector<am::Vector3f> normalData,
         std::vector<am::Vector2f> texCoordData, std::vector<unsigned int> indexData)
    {
        this->vertexData = vertexData;
        this->normalData = normalData;
        this->texCoordData = texCoordData;
        this->indexData = indexData;
        LoadBuffers();
    }
    UsualMesh(std::string filename)
    {
        NewLoadFromFile(filename);
        LoadBuffers();
    }
    ~UsualMesh()
    {
        DeleteBuffers();
    }

    void DeleteBuffers()
    {
        glDeleteVertexArrays(1, &vaobuffer);
    }
    void LoadBuffers()
    {
        int size = GetSize();
        GLenum usage;
        usage = GL_DYNAMIC_DRAW;
        glGenVertexArrays(1, &vaobuffer);
        glBindVertexArray(vaobuffer);

        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*size, (float*)GetVertexData(), usage);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(1);

        glGenBuffers(1, &texCoordBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*2*size, (float*)GetTexCoordData(), usage);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);

        glGenBuffers(1, &normalBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*size, (float*)GetNormalData(), usage);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(2);

        glGenBuffers(1, &tangBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, tangBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*size, (float*)GetTangData(), usage);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(3);

        glGenBuffers(1, &indexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indexData.size(), indexData.data(), usage);

        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glDeleteBuffers(1, &vertexBuffer);
        glDeleteBuffers(1, &texCoordBuffer);
        glDeleteBuffers(1, &normalBuffer);
        glDeleteBuffers(1, &indexBuffer);
        glDeleteBuffers(1, &tangBuffer);
    }
    bool NewLoadFromFile(std::string filename)
    {
        std::vector<am::Vector3f> vert;
        std::vector<am::Vector3f> norm;
        std::vector<am::Vector2f> tex;

        bool isThereNormal = false, isThereTexCoord = false, error = false;

        //std::ofstream ofile;
        //ofile.open(filename+"1");
        std::ifstream file;
        file.open(filename);

        std::map<IndexedVertex, int> map;
        std::map<int, std::vector<int> > nap;
        char str[256];
        while(file.getline(str, 256))
        {
            int i = 0;
            while(i<256&&str[i]==' ') i++;
            if(i>255) continue;
            float x = 0, y = 0, z = 0;
            if(str[i]=='v')
            {
                i++;
                if(i>255) continue;
                if(str[i]==' ')
                {
                    sscanf(&str[i], "%f %f %f", &x, &y, &z);
                    vert.push_back(am::Vector3f(x, y, z));
                    //ofile << "vert" << x << y << z <<"\n";
                }
                else if(str[i]=='t')
                {
                    i++;
                    if(i>255) continue;
                    isThereTexCoord = true;
                    sscanf(&str[i], "%f %f", &x, &y);
                    tex.push_back(am::Vector2f(x, y));
                    //ofile << "tex" << x << y <<"\n";
                }
                else if(str[i]=='n')
                {
                    i++;
                    if(i>255) continue;
                    isThereNormal = true;
                    sscanf(&str[i], "%f %f %f", &x, &y, &z);
                    norm.push_back(am::Vector3f(x, y, z));
                    //ofile << "norm" << x << y << z <<"\n";
                }
                else continue;
            }
            else if(str[i]=='f')
            {
                i++;
                if(i>255) continue;
                int a[4], b[4], c[4];
                IndexedVertex v(0, 0, 0);
                if(isThereNormal&&isThereTexCoord)
                {
                    int cont = sscanf(&str[i], "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", a, b, c, &a[1], &b[1], &c[1], &a[2], &b[2], &c[2], &a[3], &b[3], &c[3]);
                    for(int j = 0; j<3; j++)
                    {
                        if(a[j]<0) a[j] += vert.size()+1;
                        if(b[j]<0) b[j] += tex.size()+1;
                        if(c[j]<0) c[j] += norm.size()+1;
                        v.Update(a[j]-1, b[j]-1, c[j]-1);
                        if(map.find(v)==map.end())
                        {
                            map[v] =  map.size()-1;
                            vertexData.push_back(vert[v.vert]);
                            texCoordData.push_back(tex[v.tex]);
                            normalData.push_back(norm[v.norm]);
                        }
                        indexData.push_back(map[v]);
                        nap[indexData[indexData.size()-1]].push_back(indexData.size()-1);
                    }
                    if(cont==12)
                    {
                        if(a[3]<0) a[3] += vert.size()+1;
                        if(b[3]<0) b[3] += tex.size()+1;
                        if(c[3]<0) c[3] += norm.size()+1;
                        v.Update(a[3]-1, b[3]-1, c[3]-1);
                        if(map.find(v)==map.end())
                        {
                            map[v] =  map.size()-1;
                            vertexData.push_back(vert[v.vert]);
                            texCoordData.push_back(tex[v.tex]);
                            normalData.push_back(norm[v.norm]);
                        }
                        indexData.push_back(indexData[indexData.size()-3]);
                        nap[indexData[indexData.size()-1]].push_back(indexData.size()-1);
                        indexData.push_back(indexData[indexData.size()-2]);
                        nap[indexData[indexData.size()-1]].push_back(indexData.size()-1);
                        indexData.push_back(map[v]);
                        nap[indexData[indexData.size()-1]].push_back(indexData.size()-1);
                    }
                }
                else if(isThereNormal)
                {
                    int cont = sscanf(&str[i], "%d//%d %d//%d %d//%d %d//%d", a, c, &a[1], &c[1], &a[2], &c[2], &a[3], &c[3]);
                    tex.push_back(am::Vector2f(0, 0));
                    for(int j = 0; j<3; j++)
                    {
                        if(a[j]<0) a[j] += vert.size()+1;
                        if(c[j]<0) c[j] += norm.size()+1;
                        v.Update(a[j]-1, 0, c[j]-1);
                        if(map.find(v)==map.end())
                        {
                            map[v] =  map.size()-1;
                            vertexData.push_back(vert[v.vert]);
                            texCoordData.push_back(tex[v.tex]);
                            normalData.push_back(norm[v.norm]);
                        }
                        indexData.push_back(map[v]);
                        nap[indexData[indexData.size()-1]].push_back(indexData.size()-1);
                    }
                    if(cont==8)
                    {
                        if(a[3]<0) a[3] += vert.size()+1;
                        if(c[3]<0) c[3] += norm.size()+1;
                        v.Update(a[3]-1, 0, c[3]-1);
                        if(map.find(v)==map.end())
                        {
                            map[v] =  map.size()-1;
                            vertexData.push_back(vert[v.vert]);
                            texCoordData.push_back(tex[v.tex]);
                            normalData.push_back(norm[v.norm]);
                        }
                        indexData.push_back(indexData[indexData.size()-3]);
                        nap[indexData[indexData.size()-1]].push_back(indexData.size()-1);
                        indexData.push_back(indexData[indexData.size()-2]);
                        nap[indexData[indexData.size()-1]].push_back(indexData.size()-1);
                        indexData.push_back(map[v]);
                    }
                }
                else if(isThereTexCoord)
                {
                    int cont = sscanf(&str[i], "%d/%d %d/%d %d/%d %d/%d", a, b, &a[1], &b[1], &a[2], &b[2], &a[3], &b[3]);
                    norm.push_back(am::Vector3f(0, 0, 1));
                    for(int j = 0; j<3; j++)
                    {
                        if(a[j]<0) a[j] += vert.size()+1;
                        if(b[j]<0) b[j] += tex.size()+1;
                        v.Update(a[j]-1, b[j]-1, 0);
                        if(map.find(v)==map.end())
                        {
                            map[v] =  map.size()-1;
                            vertexData.push_back(vert[v.vert]);
                            texCoordData.push_back(tex[v.tex]);
                            normalData.push_back(norm[v.norm]);
                        }
                        indexData.push_back(map[v]);
                        nap[indexData[indexData.size()-1]].push_back(indexData.size()-1);
                    }
                    if(cont==8)
                    {
                        if(a[3]<0) a[3] += vert.size()+1;
                        if(b[3]<0) b[3] += norm.size()+1;
                        v.Update(a[3]-1, b[3]-1, 0);
                        if(map.find(v)==map.end())
                        {
                            map[v] =  map.size()-1;
                            vertexData.push_back(vert[v.vert]);
                            texCoordData.push_back(tex[v.tex]);
                            normalData.push_back(norm[v.norm]);
                        }
                        indexData.push_back(indexData[indexData.size()-3]);
                        nap[indexData[indexData.size()-1]].push_back(indexData.size()-1);
                        indexData.push_back(indexData[indexData.size()-2]);
                        nap[indexData[indexData.size()-1]].push_back(indexData.size()-1);
                        indexData.push_back(map[v]);
                        nap[indexData[indexData.size()-1]].push_back(indexData.size()-1);
                    }
                }
                else
                {
                    int cont = sscanf(&str[i], "%d %d %d %d", a, &a[1], &a[2], &a[3]);
                    norm.push_back(am::Vector3f(0, 0, 1));
                    tex.push_back(am::Vector2f(0, 0));
                    for(int j = 0; j<3; j++)
                    {
                        if(a[j]<0) a[j] += vert.size()+1;
                        v.Update(a[j]-1, 0, 0);
                        if(map.find(v)==map.end())
                        {
                            map[v] =  map.size()-1;
                            vertexData.push_back(vert[v.vert]);
                            texCoordData.push_back(tex[v.tex]);
                            normalData.push_back(norm[v.norm]);
                        }
                        indexData.push_back(map[v]);
                        nap[indexData[indexData.size()-1]].push_back(indexData.size()-1);
                    }
                    if(cont==4)
                    {
                        if(a[3]<0) a[3] += vert.size()+1;
                        v.Update(a[3]-1, 0, 0);
                        if(map.find(v)==map.end())
                        {
                            map[v] =  map.size()-1;
                            vertexData.push_back(vert[v.vert]);
                            texCoordData.push_back(tex[v.tex]);
                            normalData.push_back(norm[v.norm]);
                        }
                        indexData.push_back(indexData[indexData.size()-3]);
                        nap[indexData[indexData.size()-1]].push_back(indexData.size()-1);
                        indexData.push_back(indexData[indexData.size()-2]);
                        nap[indexData[indexData.size()-1]].push_back(indexData.size()-1);
                        indexData.push_back(map[v]);
                        nap[indexData[indexData.size()-1]].push_back(indexData.size()-1);
                    }

                }
            }
        }
        if(!isThereNormal)
            RecomputeNormals(&nap);
        else ComputeTB(&nap);
        /*for(int i = 0; i<indexData.size(); i++)
        {
            ofile << indexData[i] << " <-ind\n";
        }*/
        file.close();
        return !error;
    }

    void RecomputeNormals()
    {
        normalData.clear();
        tangData.clear();
        normalData.resize(vertexData.size());
        tangData.resize(vertexData.size());
        for(int i = 0; i < vertexData.size(); i++)
        {
            std::vector<int> a;
            for(int j = 0; j<indexData.size(); j++)
            {
                if(indexData[j]==i)
                {
                    a.push_back(j-(j%3));
                }
            }
            am::Vector3f norm, tang;
            for(int j = 0; j < a.size(); j++)
            {
                int k =  a[j]+1;
                am::Vector3f v1 = vertexData[indexData[k]]-vertexData[indexData[k+1]];
                am::Vector3f v2 = vertexData[indexData[k]]-vertexData[indexData[k+2]];
                norm+= v1^v2;
                am::Vector2f t1 = texCoordData[indexData[k]]-texCoordData[indexData[k+1]];
                am::Vector2f t2 = texCoordData[indexData[k]]-texCoordData[indexData[k+2]];
                tang+=am::Vector3f(v1.x*t2.y - t1.y*v2.x, v1.y*t2.y - t1.y*v2.y, v1.z*t2.y - t1.y*v2.z);
            }
            Normalize(norm);
            Normalize(tang);
            normalData[i] = norm;
            tangData[i] = tang;
        }
    }

    void RecomputeNormals(std::map<int, std::vector<int> > * map)
    {
        normalData.clear();
        tangData.clear();
        normalData.resize(vertexData.size());
        tangData.resize(vertexData.size());
        for(auto it = map->begin(); it!=map->end(); it++)
        {
            am::Vector3f norm(0, 0, 0);
            am::Vector3f tang(0, 0, 0);
            for(int i = 0; i < it->second.size(); i++)
            {
                int t = it->second[i];
                int k = t - (t%3);
                am::Vector3f v1 = vertexData[indexData[k]]-vertexData[indexData[k+1]];
                am::Vector3f v2 = vertexData[indexData[k]]-vertexData[indexData[k+2]];
                norm+= v1^v2;
                am::Vector2f t1 = texCoordData[indexData[k]]-texCoordData[indexData[k+1]];
                am::Vector2f t2 = texCoordData[indexData[k]]-texCoordData[indexData[k+2]];
                tang+=am::Vector3f(v1.x*t2.y - t1.y*v2.x, v1.y*t2.y - t1.y*v2.y, v1.z*t2.y - t1.y*v2.z);
            }
            Normalize(norm);
            Normalize(tang);
            normalData[it->first] = norm;
            tangData[it->first] = tang;
        }
    }
    void ComputeTB(std::map<int, std::vector<int> > * map)
    {
        tangData.clear();
        tangData.resize(vertexData.size());
        for(auto it = map->begin(); it!=map->end(); it++)
        {
            am::Vector3f tang(0, 0, 0);
            am::Vector3f bitang(0, 0, 0);
            for(int i = 0; i < it->second.size(); i++)
            {
                int t = it->second[i];
                int k = t - (t%3);
                am::Vector3f v1 = vertexData[indexData[k]]-vertexData[indexData[k+1]];
                am::Vector3f v2 = vertexData[indexData[k]]-vertexData[indexData[k+2]];
                am::Vector2f t1 = texCoordData[indexData[k]]-texCoordData[indexData[k+1]];
                am::Vector2f t2 = texCoordData[indexData[k]]-texCoordData[indexData[k+2]];
                tang+=am::Vector3f(v1.x*t2.y - t1.y*v2.x, v1.y*t2.y - t1.y*v2.y, v1.z*t2.y - t1.y*v2.z);
            }
            Normalize(tang);
            Normalize(bitang);
            tangData[it->first] = tang;
        }
    }
    std::vector<am::Vector3f> normalData;
    std::vector<am::Vector2f> texCoordData;
    std::vector<unsigned int> indexData;
    std::vector<am::Vector3f> tangData;
    friend class Renderer;
    friend class Application;
};
};

#pragma once
#include "DecartVector.hpp"
#include <cmath>
#include <iostream>

namespace am
{
template<typename T>
class Matrix3x3
{
public:
    Matrix3x3(T x=0)
    {
        SetDiagonal(x);
    }
    Matrix3x3(am::Vector3<T> v1, am::Vector3<T> v2, am::Vector3<T> v3)
    {
        vec[0] = v1;
        vec[1] = v2;
        vec[2] = v3;
    }
    am::Vector3<T> & operator [] (int i)
    {
        return vec[i];
    }
    void FillMatrix(T x)
    {
        for(int i = 0; i<3; i++)
        {
            for(int j = 0; j<3; j++)
            {
                vec[i][j] = x;
            }
        }
    }
    void Transpose()
    {
        for(int i = 0; i<2; i++)
        {
            for(int j = i+1; j<3; j++)
            {
                T k = vec[i][j];
                vec[i][j] = vec[j][i];
                vec[j][i] = k;
            }
        }
    }
    void SetDiagonal(T x)
    {
        FillMatrix(0);
        for(int i = 0; i<3; i++)
        {
            vec[i][i] = x;
        }
    }
    void SetIdentity()
    {
        SetDiagonal(1);
    }
    void Print()
    {
        std::cout << "________\n";
        for(int i = 0; i < 3; i++)
        {
            std::cout << "| ";
            for(int j = 0; j < 3; j++)
                std::cout << vec[i][j] << " ";
            std::cout << "|\n";
        }

    }
private:
    am::Vector3<T> vec[3];
};

template<typename T>
Matrix3x3<T> Transpose(Matrix3x3<T> m)
{
    Matrix3x3<T> r;
    r = m;
    r.Transpose();
    return r;
}

template<typename T>
Matrix3x3<T> operator * (Matrix3x3<T> m, float k)
{
    Matrix3x3<T> r;
    for(int i = 0; i<3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            r[i][j] = m[i][j]*k;
        }
    }
    return r;
}

template<typename T>
Matrix3x3<T> operator * (float k, Matrix3x3<T> m)
{
    Matrix3x3<T> r;
    for(int i = 0; i<3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            r[i][j] = m[i][j]*k;
        }
    }
    return r;
}

template<typename T>
am::Vector3<T> operator * (Matrix3x3<T> m, am::Vector3<T> v)
{
    am::Vector3<T> r;
    for(int i = 0; i<3; i++)
    {
        T k = 0;
        for(int j = 0; j < 3; j++)
        {
            k+=m[i][j]*v[j];
        }
        r[i] = k;
    }
    return r;
}

template<typename T>
am::Vector3<T> operator * (am::Vector3<T> v, Matrix3x3<T> m)
{
    am::Vector3<T> r;
    for(int i = 0; i<3; i++)
    {
        T k = 0;
        for(int j = 0; j < 3; j++)
        {
            k+=m[j][i]*v[j];
        }
        r[i] = k;
    }
    return r;
}

template<typename T>
Matrix3x3<T> operator * (Matrix3x3<T> a, Matrix3x3<T> b)
{
    Matrix3x3<T> r;
    for(int i = 0; i<3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            T t = 0;
            for(int k = 0; k<3; k++)
            {
                t+=a[i][k]*b[k][j];
            }
            r[i][j] = t;
        }
    }
    return r;
}

template<typename T>
Matrix3x3<T> operator + (Matrix3x3<T> a, Matrix3x3<T> b)
{
    Matrix3x3<T> r;
    for(int i = 0; i<3; i++)
    {
        for(int j = 0; j<3; j++)
        {
            r[i][j] = a[i][j]+b[i][j];
        }
    }
    return r;
}

template<typename T>
Matrix3x3<T> GetCrossProductMatrix(am::Vector3<T> v)
{
    return Matrix3x3<T>(am::Vector3<T>(0, -v[2], v[1]),am::Vector3<T>(v[2], 0, -v[0]), am::Vector3<T>(-v[1], v[0], 0));
}

template<typename T>
Matrix3x3<T> GetTensorProduct(am::Vector3<T> a, am::Vector3<T> b)
{
    return Matrix3x3<T>(am::Vector3<T>(a.x*b.x, a.x*b.y, a.x*b.z),am::Vector3<T>(a.y*b.x, a.y*b.y, a.y*b.z),
                      am::Vector3<T>(a.z*b.x, a.z*b.y, a.z*b.z));
}

#define Matrix3x3f Matrix3x3<float>

template<typename T>
class Matrix4x4
{
public:
    Matrix4x4(T x=0)
    {
        FillMatrix(0);
        for(int i = 0; i<4; i++)
        {
            vec[i][i] = x;
        }
    }
    Matrix4x4(am::Vector4<T> v1, am::Vector4<T> v2, am::Vector4<T> v3, am::Vector4<T> v4)
    {
        vec[0] = v1;
        vec[1] = v2;
        vec[2] = v3;
        vec[3] = v4;
    }

    am::Vector4<T>& operator [] (int i)
    {
        return vec[i];
    }
    void FillMatrix(T x)
    {
        for(int i = 0; i<4; i++)
        {
            vec[i] = am::Vector4f();
        }
    }
    void Transpose()
    {
        for(int i = 0; i < 3; i++)
        {
            for(int j = i+1; j<4; j++)
            {
                T t = vec[i][j];
                vec[i][j] = vec[j][i];
                vec[j][i] = t;
            }
        }
    }
    void SetDiagonal(T x)
    {
        FillMatrix(0);
        for(int i = 0; i<4; i++)
        {
            vec[i][i] = x;
        }
    }
    void SetIdentity()
    {
        SetDiagonal(1);
    }

    void Print()
    {
        std::cout << "___________\n";
        for(int i = 0; i < 4; i++)
        {
            std::cout << "| ";
            for(int j = 0; j < 4; j++)
                std::cout << vec[i][j] << " ";
            std::cout << "|\n";
        }

    }

private:
    am::Vector4<T> vec[4];
};

template<typename T>
Matrix4x4<T> operator * (Matrix4x4<T> a, Matrix4x4<T> b)
{
    Matrix4x4<T> m;
    for(int i = 0; i<4; i++)
        for(int j = 0; j<4; j++)
        {
            T k = 0;
            for( int t = 0; t<4; t++)
            {
                k+=a[i][t]*b[t][j];
            }
            m[i][j]=k;
        }
    return m;
}

template<typename T>
Matrix4x4<T> Transpose(Matrix4x4<T> m)
{
    Matrix4x4<T> r;
    r = m;
    r.Transpose();
    return r;
}

template<typename T>
Matrix4x4<T> operator *= (Matrix4x4<T> & a, Matrix4x4<T> b)
{
    Matrix4x4<T> m;
    for(int i = 0; i<4; i++)
        for(int j = 0; j<4; j++)
        {
            int k = 0;
            for( int t = 0; t<4; t++)
            {
                k+=a[i][k]*b[k][j];
            }
            m[i][j]=k;
        }
    a = m;
    return a;
}
template<typename T>
am::Vector4<T> operator * (Matrix4x4<T> m, am::Vector4<T> v)
{
    am::Vector4<T> res;
    for(int i = 0; i<4; i++)
    {
        float t = 0;
        for(int k = 0; k<4; k++)
        {
            t+=m[i][k]*v[k];
        }
        res[i] = t;
    }
    return res;
}

#define Matrix4x4f Matrix4x4<float>
};

#pragma once
#include <cmath>
#include <iostream>
#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"

namespace am
{
template<typename T>
class Vector2
{
public:
    T x, y;

    Vector2(T x=0, T y=0)
    {
        this->x = x;
        this->y = y;
    }
    T GetSqrLength()
    {
        return x*x+y*y;
    }
    void Print()
    {
        std::cout << "{ " << x << ", " << y << " }" << "\n";
    }
    bool IsBelongToRect(Vector2<T> min, Vector2<T> max)
    {
        if((this->x<max.x)&&(this->y<max.y)&&(this->x>min.x)&&(this->y>min.y))
            return true;
        else return false;
    }
};
template <typename T>
Vector2<T> operator +(Vector2<T> a, Vector2<T> b)
{
    Vector2<T> c;
    c.x = a.x+b.x;
    c.y = a.y+b.y;
    return c;
}
template <typename T>
Vector2<T> operator -(Vector2<T> a, Vector2<T> b)
{
    Vector2<T> c;
    c.x = a.x-b.x;
    c.y = a.y-b.y;
    return c;
}
template <typename T>
T operator *(Vector2<T> a, Vector2<T> b)
{
    T c;
    c = a.x*b.x+a.y*b.y;
    return c;
}
template <typename T>
Vector2<T> operator *(Vector2<T> v, T a)
{
    Vector2<T> r;
    r.x = v.x*a;
    r.y = v.y*a;
    return r;
}
template <typename T>
Vector2<T> operator *(T a, Vector2<T> v)
{
    Vector2<T> r;
    r.x = v.x*a;
    r.y = v.y*a;
    return r;
}
template <typename T>
T operator ^(Vector2<T> a, Vector2<T> b)
{
    T c;
    c = a.x*b.y - a.y*b.x;
    return c;
}
template <typename T>
bool operator <(Vector2<T> a, Vector2<T> b)
{
    if((a.x<b.x)&&(a.y<b.y)) return true;
    else return false;
}
template <typename T>
bool operator > (Vector2<T> a, Vector2<T> b)
{
    if((a.x>b.x)&&(a.y>b.y)) return true;
    else return false;
}
template<typename T>
Vector2<T> DualTo(Vector2<T> vec)
{
    return Vector2<T>(vec.x, -vec.y);
}

template <typename T>
int operator +=(Vector2<T> &v1, Vector2<T> v2)
{
    v1.x+=v2.x;
    v1.y+=v2.y;
    return 0;
}

#define Vector2i Vector2<int>
#define Vector2f Vector2<float>
#define Vector2d Vector2<double>

template<typename T>
class Vector3
{
public:
    T x, y, z;

    Vector3(T x=0, T y=0, T z=0)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    T & operator [] (int i)
    {
        return ((T*)this)[i];
    }

    T GetSqrLength()
    {
        return x*x+y*y+z*z;
    }
    void Print()
    {
        std::cout << "{ " << x << ", " << y << ", " << z << " }" << "\n";
    }
};

template <typename T>
bool operator < (const Vector3<T> &v1, const Vector3<T> &v2)
{
    if(v1.x<v2.x) return true;
    if(v1.x>v2.x) return false;
    if(v1.y<v2.y) return true;
    if(v1.y>v2.y) return false;
    if(v1.z<v2.z) return true;
    else return false;
}

template <typename T>
Vector3<T> operator +(Vector3<T> a, Vector3<T> b)
{
    Vector3<T> c;
    c.x = a.x+b.x;
    c.y = a.y+b.y;
    c.z = a.z+b.z;
    return c;
}
template <typename T>
Vector3<T> operator -(Vector3<T> a, Vector3<T> b)
{
    Vector3<T> c;
    c.x = a.x-b.x;
    c.y = a.y-b.y;
    c.z = a.z-b.z;
    return c;
}
template <typename T>
T operator *(Vector3<T> a, Vector3<T> b)
{
    T c;
    c = a.x*b.x+a.y*b.y+a.z*b.z;
    return c;
}
template <typename T>
Vector3<T> operator *(Vector3<T> v, T a)
{
    Vector3<T> r;
    r.x = v.x*a;
    r.y = v.y*a;
    r.z = v.z*a;
    return r;
}
template <typename T>
Vector3<T> operator *(T a, Vector3<T> v)
{
    Vector3<T> r;
    r.x = v.x*a;
    r.y = v.y*a;
    r.z = v.z*a;
    return r;
}
template <typename T>
Vector3<T> operator ^(Vector3<T> a, Vector3<T> b)
{
    Vector3<T> c;
    c.x = a.y*b.z - a.z*b.y;
    c.y = a.z*b.x - a.x*b.z;
    c.z = a.x*b.y - a.y*b.x;
    return c;
}
template <typename T>
Vector3<T> operator += (Vector3<T> &v1, Vector3<T> v2)
{
    v1.x+=v2.x;
    v1.y+=v2.y;
    v1.z+=v2.z;
    return v1;
}
template<typename T>
Vector3<T> operator *=(Vector3<T> &v, T k)
{
    v.x*=k;
    v.y*=k;
    v.z*=k;
    return v;
}

#define Vector3i Vector3<int>
#define Vector3f Vector3<float>
#define Vector3d Vector3<double>


template<typename T>
class Vector4
{
public:
    T x, y, z, w;

    Vector4(T x=0, T y=0, T z=0, T w=0)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }
    Vector4(T* data)
    {
        this->x = data[0];
        this->y = data[1];
        this->z = data[2];
        this->w = data[3];
    }
    Vector4(Vector3<T> v, T w)
    {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
        this->w = w;
    }

    T GetSqrLength()
    {
        return x*x+y*y+z*z+w*w;
    }
    void Print()
    {
        std::cout << "{ " << x << ", " << y << ", " << z << ", " << w << " }" << "\n";
    }
    T & operator [](int i)
    {
        return  (((T*)this)[i]);
    }
};

#define Vector4i Vector4<int>
#define Vector4f Vector4<float>
#define Vector4d Vector4<double>

bool Normalize(Vector4f &v)
{
    float l = v.GetSqrLength();
    if(l==0) return false;
    l = std::sqrt(l);
    l = 1.f/l;
    for(int i = 0; i<4; i++)
    {
        v[i]*=l;
    }
    return true;
}
bool Normalize(Vector3f &v)
{
    float l = v.GetSqrLength();
    if(l==0) return false;
    l = std::sqrt(l);
    l = 1.f/l;
    v.x*=l;
    v.y*=l;
    v.z*=l;
    return true;
}

template<typename T>
Vector3<T> Vec3(Vector4<T> v)
{
    Vector3<T> r;
    r.x = v.x;
    r.y = v.y;
    r.z = v.z;
    return r;
}
};

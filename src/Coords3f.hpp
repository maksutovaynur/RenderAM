#include "DecartVector.hpp"
#include "Matrices.hpp"
namespace am
{
struct Coords3f
{
public:
    Coords3f(am::Vector3f pos = am::Vector3f(0, 0, 0), am::Vector3f xVector = am::Vector3f(1, 0, 0), am::Vector3f yVector = am::Vector3f(0, 1, 0))
    {
        this->pos = pos;
        SetOrientation(xVector, yVector);
    }
    void SetOrientation(am::Vector3f xVector = am::Vector3f(1, 0, 0), am::Vector3f yVector = am::Vector3f(0, 1, 0))
    {
        Normalize(xVector);
        Normalize(yVector);
        am::Vector3f zVector = xVector^yVector;
        Normalize(zVector);
        yVector = zVector^xVector;
        Normalize(yVector);
        rotationMatrix = Matrix3x3f(xVector, yVector, zVector);
        rotationMatrix.Transpose();
    }
    void Transform(Matrix3x3f m)
    {
        rotationMatrix = m*rotationMatrix;
    }
    void GlobalTranslate(am::Vector3f v, float dl)
    {
        am::Vector3f vec = v;
        if(Normalize(vec))
        {
            vec*=dl;
            pos+=vec;
        }
    }
    void LocalTranslate(am::Vector3f v, float dl)
    {
        am::Vector3f vec = rotationMatrix*v;
        if(Normalize(vec))
        {
            vec*=dl;
            pos+=vec;
        }
    }
    void GlobalRotate(am::Vector3f v, float rad)
    {
        am::Vector3f vec = v;
        if(Normalize(vec))
        {
            float sa = sin(rad);
            float ca = cos(rad);
            Matrix3x3f i(1);
            Matrix3x3f c = GetCrossProductMatrix(vec);
            Matrix3x3f t = GetTensorProduct(vec, vec);
            Matrix3x3f d = i*ca+c*sa+(1-ca)*t;
            rotationMatrix = d*rotationMatrix;
        }
    }
    void LocalRotate(am::Vector3f v, float rad)
    {
        am::Vector3f vec = rotationMatrix*v;
        if(Normalize(vec))
        {
            float sa = sin(rad);
            float ca = cos(rad);
            Matrix3x3f i(1);
            Matrix3x3f c = GetCrossProductMatrix(vec);
            Matrix3x3f t = GetTensorProduct(vec, vec);
            Matrix3x3f d = i*ca+c*sa+(1-ca)*t;
            rotationMatrix = d*rotationMatrix;
        }
    }
    void GlobalScale(am::Vector3f scale)
    {
        Matrix3x3f m(am::Vector3f(scale.x, 0, 0), am::Vector3f(0, scale.y, 0), am::Vector3f(0, 0, scale.z));
        rotationMatrix = m*rotationMatrix;
    }
    void LocalScale(am::Vector3f scale)
    {
        Matrix3x3f m(am::Vector3f(scale.x, 0, 0), am::Vector3f(0, scale.y, 0), am::Vector3f(0, 0, scale.z));
        m = rotationMatrix*m;
        rotationMatrix = m*rotationMatrix;
    }

    void CutLessThan(float v)
    {
        for(int i = 0; i < 3; i++)
        {
            for(int j = 0; j < 3; j++)
            {
                if(rotationMatrix[i][j]*rotationMatrix[i][j]<v*v) rotationMatrix[i][j] = 0;
            }
        }
    }

    am::Matrix3x3f GetMatrix()
    {
        return rotationMatrix;
    }
    am::Vector3f GetLocalPoint(am::Vector3f worldPoint)
    {
        return (worldPoint-pos)*rotationMatrix;
    }
    am::Vector3f GetWorldPoint(am::Vector3f localPoint)
    {
        return pos+(rotationMatrix*localPoint);
    }
    am::Vector3f pos;
private:
    Matrix3x3f rotationMatrix;
};

};

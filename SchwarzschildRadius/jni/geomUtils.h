#ifndef _GEOMUTILS_H_
#define _GEOMUTILS_H_
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include "geomTypedefs.h"
#include "utils.h"
#define computeIndex(x,y, rows) (x * rows + y)

namespace geomUtils
{
    static const float gFuzz = 0.0000001f;
    void frustum(float left, float right, float top, float bottom, float znear, float zfar, tVectorOfFloats& matrix);
    void perspective(float fovyInDegrees, float aspectRatio, float znear, float zfar, tVectorOfFloats& matrix);
    void ComputeNormalOfPlane(const tVectorOfFloats& vector1, const tVectorOfFloats& vector2, const tVectorOfFloats& normal);
    void NormalizeVector(tVectorOfFloats& vector);
    inline
    void makeIdentityMatrix(tVectorOfFloats& result) {
        result.resize(16);
        std::fill(result.begin(), result.end(), 0.0f);
        result[0 * 4 + 0] = 1.0f;
        result[1 * 4 + 1] = 1.0f;
        result[2 * 4 + 2] = 1.0f;
        result[3 * 4 + 3] = 1.0f;
    }
    
    inline
    bool scale(float sx, float sy, float sz, tVectorOfFloats& result) {
        if (result.size() < 4*4)
            return false;
        result[0 * 4 + 0] *= sx;
        result[0 * 4 + 1] *= sx;
        result[0 * 4 + 2] *= sx;
        result[0 * 4 + 3] *= sx;
        
        result[1 * 4 + 0] *= sy;
        result[1 * 4 + 1] *= sy;
        result[1 * 4 + 2] *= sy;
        result[1 * 4 + 3] *= sy;
        
        result[2 * 4 + 0] *= sz;
        result[2 * 4 + 1] *= sz;
        result[2 * 4 + 2] *= sz;
        result[2 * 4 + 3] *= sz;
        return true;
    }
    
    inline
    bool translate(float tx, float ty, float tz, tVectorOfFloats& result) 
    {
        if (result.size() < 4*4)
            return false;
        result[3 * 4 + 0] += (result[0 * 4 + 0] * tx + result[1 * 4 + 0] * ty + result[2 * 4 + 0] * tz);
        result[3 * 4 + 1] += (result[0 * 4 + 1] * tx + result[1 * 4 + 1] * ty + result[2 * 4 + 1] * tz);
        result[3 * 4 + 2] += (result[0 * 4 + 2] * tx + result[1 * 4 + 2] * ty + result[2 * 4 + 2] * tz);
        result[3 * 4 + 3] += (result[0 * 4 + 3] * tx + result[1 * 4 + 3] * ty + result[2 * 4 + 3] * tz);
        return true;
    }
    
    inline
    void matrixMultiply(const tVectorOfFloats& srcA, const tVectorOfFloats& srcB, tVectorOfFloats& result) {
        int i;
        tVectorOfFloats tmp(16);
        makeIdentityMatrix(tmp);
        for (i = 0; i < 4; i++) {
            tmp[i * 4 + 0] = (srcA[i * 4 + 0] * srcB[0 * 4 + 0])
            + (srcA[i * 4 + 1] * srcB[1 * 4 + 0])
            + (srcA[i * 4 + 2] * srcB[2 * 4 + 0])
            + (srcA[i * 4 + 3] * srcB[3 * 4 + 0]);
            
            tmp[i * 4 + 1] = (srcA[i * 4 + 0] * srcB[0 * 4 + 1])
            + (srcA[i * 4 + 1] * srcB[1 * 4 + 1])
            + (srcA[i * 4 + 2] * srcB[2 * 4 + 1])
            + (srcA[i * 4 + 3] * srcB[3 * 4 + 1]);
            
            tmp[i * 4 + 2] = (srcA[i * 4 + 0] * srcB[0 * 4 + 2])
            + (srcA[i * 4 + 1] * srcB[1 * 4 + 2])
            + (srcA[i * 4 + 2] * srcB[2 * 4 + 2])
            + (srcA[i * 4 + 3] * srcB[3 * 4 + 2]);
            
            tmp[i * 4 + 3] = (srcA[i * 4 + 0] * srcB[0 * 4 + 3])
            + (srcA[i * 4 + 1] * srcB[1 * 4 + 3])
            + (srcA[i * 4 + 2] * srcB[2 * 4 + 3])
            + (srcA[i * 4 + 3] * srcB[3 * 4 + 3]);
        }
        result = tmp;
    }
    
    inline
    void rotate(float angle, float x, float y, float z, tVectorOfFloats& result) {
        float sinAngle, cosAngle;
        float mag = (float) sqrt((double) (x * x + y * y + z * z));
        sinAngle = (float) sin((double) (angle * PI / 180.0));
        cosAngle = (float) cos((double) (angle * PI / 180.0));
        if (mag > 0.0f) {
            float xx, yy, zz, xy, yz, zx, xs, ys, zs;
            float oneMinusCos;
            tVectorOfFloats rotMat(16);
            
            x /= mag;
            y /= mag;
            z /= mag;
            
            xx = x * x;
            yy = y * y;
            zz = z * z;
            xy = x * y;
            yz = y * z;
            zx = z * x;
            xs = x * sinAngle;
            ys = y * sinAngle;
            zs = z * sinAngle;
            oneMinusCos = 1.0f - cosAngle;
            
            rotMat[0 * 4 + 0] = (oneMinusCos * xx) + cosAngle;
            rotMat[0 * 4 + 1] = (oneMinusCos * xy) - zs;
            rotMat[0 * 4 + 2] = (oneMinusCos * zx) + ys;
            rotMat[0 * 4 + 3] = 0.0F;
            
            rotMat[1 * 4 + 0] = (oneMinusCos * xy) + zs;
            rotMat[1 * 4 + 1] = (oneMinusCos * yy) + cosAngle;
            rotMat[1 * 4 + 2] = (oneMinusCos * yz) - xs;
            rotMat[1 * 4 + 3] = 0.0F;
            
            rotMat[2 * 4 + 0] = (oneMinusCos * zx) - ys;
            rotMat[2 * 4 + 1] = (oneMinusCos * yz) + xs;
            rotMat[2 * 4 + 2] = (oneMinusCos * zz) + cosAngle;
            rotMat[2 * 4 + 3] = 0.0F;
            
            rotMat[3 * 4 + 0] = 0.0F;
            rotMat[3 * 4 + 1] = 0.0F;
            rotMat[3 * 4 + 2] = 0.0F;
            rotMat[3 * 4 + 3] = 1.0F;
            
            matrixMultiply(rotMat, result, result);
        }
    }

inline
bool gluFrustum(GLfloat left, GLfloat right,
              GLfloat top, GLfloat bottom, 
              GLfloat nearval, GLfloat farval,
              tVectorOfFloats& resultMatrix)
{
   GLfloat x, y, a, b, c, d;
   tVectorOfFloats m(16);

    if (std::abs(farval-nearval) <= gFuzz)
        return false;

   x = (2.0f*nearval) / (right-left);
   y = (2.0f*nearval) / (top-bottom);
   a = (right+left) / (right-left);
   b = (top+bottom) / (top-bottom);
   c = -(farval+nearval) / ( farval-nearval);
   d = -(2.0f*farval*nearval) / (farval-nearval);  

   m[computeIndex(0,0,4)] = x;     m[computeIndex(0,1,4)] = 0.0F;  m[computeIndex(0,2,4)] = a;      m[computeIndex(0,3,4)] = 0.0f;
   m[computeIndex(1,0,4)] = 0.0f;  m[computeIndex(1,1,4)] = y;     m[computeIndex(1,2,4)] = b;      m[computeIndex(1,3,4)] = 0.0f;
   m[computeIndex(2,0,4)] = 0.0f;  m[computeIndex(2,1,4)] = 0.0F;  m[computeIndex(2,2,4)] = c;      m[computeIndex(2,3,4)] = d;
   m[computeIndex(3,0,4)] = 0.0f;  m[computeIndex(3,1,4)] = 0.0F;  m[computeIndex(3,2,4)] = -1.0f;  m[computeIndex(3,3,4)] = 0.0f;

   matrixMultiply( resultMatrix, m, resultMatrix); //apply the projection matrix to the result matrix
   return true;
}

inline
void gluPerspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar, tVectorOfFloats& resultMatrix)
{
    double sine, cotangent, deltaZ;
    double radians = fovy / 2 * PI / 180;
    tVectorOfFloats workArea;
    workArea.resize(16);
    (workArea);

    deltaZ = zFar - zNear;
    sine = std::sin(radians);
    if ((deltaZ == 0) || (sine == 0) || (aspect == 0)) 
        return;
    
    cotangent = std::cos(radians) / sine;

    makeIdentityMatrix(workArea);
    size_t l = 4;
    workArea[computeIndex(0,0,l)] = cotangent / aspect;
    workArea[computeIndex(1,1,l)] = cotangent;
    workArea[computeIndex(2,2,l)] = -(zFar + zNear) / deltaZ;
    workArea[computeIndex(2,3,l)] = -1;
    workArea[computeIndex(3,2,l)] = -2 * zNear * zFar / deltaZ;
    workArea[computeIndex(3,3,l)] = 0;
    matrixMultiply(workArea, resultMatrix, resultMatrix);
}
    
    inline
    void perspective(float fovyInDegrees, float aspectRatio, float znear, float zfar, tVectorOfFloats& matrix)
    {
        float ymax, xmax;
        float temp, temp2, temp3, temp4;
        ymax = znear * tanf(fovyInDegrees * M_PI / 360.0);
        xmax = ymax * aspectRatio;
        frustum(-xmax, xmax, -ymax, ymax, znear, zfar, matrix);
    }
    inline
    void frustum(float left, float right, float top, float bottom, float znear, float zfar, tVectorOfFloats& matrix)
    {
        float temp, temp2, temp3, temp4;
        temp = 2.0 * znear;
        temp2 = right - left;
        temp3 = top - bottom;
        temp4 = zfar - znear;
        matrix[0] = temp / temp2;
        matrix[1] = 0.0;
        matrix[2] = 0.0;
        matrix[3] = 0.0;
        matrix[4] = 0.0;
        matrix[5] = temp / temp3;
        matrix[6] = 0.0;
        matrix[7] = 0.0;
        matrix[8] = (right + left) / temp2;
        matrix[9] = (top + bottom) / temp3;
        matrix[10] = (-zfar - znear) / temp4;
        matrix[11] = -1.0;
        matrix[12] = 0.0;
        matrix[13] = 0.0;
        matrix[14] = (-temp * zfar) / temp4;
        matrix[15] = 0.0;
    }
    inline void ComputeNormalOfPlane(const tVectorOfFloats& vector1, const tVectorOfFloats& vector2, tVectorOfFloats& normal)
    {
        normal[0]=(vector1[1]*vector2[2])-(vector1[2]*vector2[1]);
        normal[1]=(vector1[2]*vector2[0])-(vector1[0]*vector2[2]);
        normal[2]=(vector1[0]*vector2[1])-(vector1[1]*vector2[0]);
    }
    inline void NormalizeVector(tVectorOfFloats& vector)
    {
        float denum = 1.0/sqrt(vector[0]*vector[0]+vector[1]*vector[1]+vector[2]*vector[2]);
        vector[0]*=denum;
        vector[1]*=denum;
        vector[2]*=denum;
    }
  
inline
void gluLookAt(const tVectorOfFloats& eye, const tVectorOfFloats& center, const tVectorOfFloats& cameraUp, const tVectorOfFloats& projection, tVectorOfFloats& resultMatrix)
{
    tVectorOfFloats forward(3);
    tVectorOfFloats side(3);
    tVectorOfFloats up(3);
    tVectorOfFloats m(16);

    forward[0] = center[0] - eye[0];
    forward[1] = center[1] - eye[1];
    forward[2] = center[2] - eye[2];

    up[0] = cameraUp[0];
    up[1] = cameraUp[1];
    up[2] = cameraUp[2];

    NormalizeVector(forward);


    /* Side = forward x up */
    ComputeNormalOfPlane(forward, up, side);
    NormalizeVector(side);

    /* Recompute up as: up = side x forward */
    ComputeNormalOfPlane(side, forward, up);

    makeIdentityMatrix(m);
    m[computeIndex(0,0,4)] = side[0];
    m[computeIndex(1,0,4)] = side[1];
    m[computeIndex(2,0,4)] = side[2];

    m[computeIndex(0,1,4)] = up[0];
    m[computeIndex(1,1,4)] = up[1];
    m[computeIndex(2,2,4)] = up[2];

    m[computeIndex(0,2,4)] = -forward[0];
    m[computeIndex(1,2,4)] = -forward[1];
    m[computeIndex(2,2,4)] = -forward[2];

    matrixMultiply(m, projection, resultMatrix);
    translate(-eye[0], -eye[1], -eye[2], resultMatrix);
}
inline
    void lookAt(const tVectorOfFloats& eyePosition3D, const tVectorOfFloats& center3D, const tVectorOfFloats& upVector3D, tVectorOfFloats& matrix)
    {
        tVectorOfFloats forward(3), side(3), up(3);
        tVectorOfFloats matrix2(16), resultMatrix(16);
        std::fill(forward.begin(), forward.end(), 0.0f);
        std::fill(side.begin(), side.end(), 0.0f);
        std::fill(up.begin(), up.end(), 0.0f);
        std::fill(matrix2.begin(), matrix2.end(), 0.0f);
        std::fill(resultMatrix.begin(), resultMatrix.end(), 0.0f);
        //------------------
        forward[0] = center3D[0] - eyePosition3D[0];
        forward[1] = center3D[1] - eyePosition3D[1];
        forward[2] = center3D[2] - eyePosition3D[2];
        NormalizeVector(forward);
        //------------------
        //Side = forward x up
        ComputeNormalOfPlane(forward, upVector3D, side);
        NormalizeVector(side);
        //------------------
        //Recompute up as: up = side x forward
        ComputeNormalOfPlane(side, forward, up);
        //------------------
        matrix2[0] = side[0];
        matrix2[4] = side[1];
        matrix2[8] = side[2];
        matrix2[12] = 0.0;
        //------------------
        matrix2[1] = up[0];
        matrix2[5] = up[1];
        matrix2[9] = up[2];
        matrix2[13] = 0.0;
        //------------------
        matrix2[2] = -forward[0];
        matrix2[6] = -forward[1];
        matrix2[10] = -forward[2];
        matrix2[14] = 0.0;
        //------------------
        matrix2[3] = matrix2[7] = matrix2[11] = 0.0;
        matrix2[15] = 1.0;
        //------------------
        matrixMultiply(matrix, matrix2, resultMatrix);
        translate(-eyePosition3D[0], -eyePosition3D[1], -eyePosition3D[2], resultMatrix);
        //------------------
        memcpy(&matrix[0], &resultMatrix[0], resultMatrix.size()* sizeof(tVectorOfFloats::value_type));
    }    
    struct rotationOperator
    {
        float m_angle;
        tVectorOfFloats& m_matrix;
        tVectorOfFloats m_rotationAngle, m_rate;
        rotationOperator(tVectorOfFloats& matrix, float xRate, float yRate) : m_angle(0.0f), m_matrix(matrix)
        {
            m_rotationAngle.resize(3);
            m_rate.resize(3);
            std::fill(m_rotationAngle.begin(), m_rotationAngle.end(), 0.0f);
            m_rate[0] = xRate;
            m_rate[1] = yRate;
        }
        void operator() (const tVectorOfFloats& axisVector, int axis, const Point<GLfloat>& head, const Point<GLfloat>& tail, float delta)
        {
            switch(axis){
                case 1:
                    m_rotationAngle[0]=delta;
                    rotate(m_rotationAngle[0]*m_rate[0], axisVector[0], axisVector[1], axisVector[2], m_matrix);
                    break;
                    
                case 2:
                    m_rotationAngle[1]=delta;
                    rotate(m_rotationAngle[1]*m_rate[1], axisVector[0], axisVector[1], axisVector[2], m_matrix);
                    break;
                    
                case 3:
                    rotate(m_rotationAngle[2]*m_rate[2], axisVector[0], axisVector[1], axisVector[2],  m_matrix);
                    break;
                    
                default:
                    LOGD("=-=-=-=-= from rotationOperator", "message 4");
                    m_rotationAngle[0]=m_rotationAngle[1]=0.0f;
                    break;
            }
        }
    };
    
    struct cameraTranslationOperator
    {
        const tVectorOfFloats& m_projection;
        const tVectorOfFloats& m_view;
        tVectorOfFloats& m_resultMatrix;
        
        tVectorOfFloats m_translationVector, m_rate;
        tVectorOfFloats m_eyeVector;        /*eye vector*/ 
        tVectorOfFloats m_centerVector;     /*center of projection*/
        tVectorOfFloats m_upVector;         /*camera orientation*/

        cameraTranslationOperator(const tVectorOfFloats& view, const tVectorOfFloats& projection, tVectorOfFloats& resultVector, float xRate, float yRate) 
            : m_projection(projection), m_view(view), m_resultMatrix(resultVector)
        {
            m_translationVector.resize(3);
            m_rate.resize(3);
            m_eyeVector.resize(3);
            std::fill(m_translationVector.begin(), m_translationVector.end(), 0.0f);
            m_rate[0] = xRate;
            m_rate[1] = yRate;
            m_centerVector.resize(3);
            m_upVector.resize(3);
            m_eyeVector.resize(3);
            std::fill(m_centerVector.begin(), m_centerVector.end(), 0.0f);
            std::fill(m_upVector.begin(), m_upVector.end(), 0.0f);
            std::fill(m_eyeVector.begin(), m_eyeVector.end(), 0.0f);
            m_eyeVector[2] = -1.0f;
            m_upVector[1] = 1.0f;

        }
        //TODO: not using axisVector, which determines the axis of change, i.e. 0,1,0 means y is changing
        void operator() (geomUtils::tVectorOfFloats& axisVector, int axis, const Point<GLfloat>& head, const Point<GLfloat>& tail, float delta)
        {
            switch(axis){
                case 1:
                {
                    m_eyeVector[0] = head.x;
                    gluLookAt (/*eye vector*/    m_eyeVector, 
                               /*center vector*/ m_centerVector, 
                               /*up vector*/     m_upVector, 
                               /*model view*/    m_projection,
                               /*the computed view matrix*/m_resultMatrix);    //create the view matrix
                }
                break;

                case 2:
                {
                    m_eyeVector[1] = head.y;
                    gluLookAt (/*eye vector*/    m_eyeVector, 
                               /*center vector*/ m_centerVector, 
                               /*up vector*/     m_upVector, 
                               /*model view*/    m_projection,
                               /*the computed view matrix*/m_resultMatrix);    //create the view matrix
                }
                    
                default:
                    break;
            }
        }
    };
    template <typename T>
    inline 
    bool computeMinMax3DLinear(const std::vector<T>& source, Vector<T>& vecMin, Vector<T>& vecMax)
    {
        if (source.size() == 0 || source.size() % 3 != 0) return false; //the source vector must not be empty and it must contain a number of items that is a multiple of three
        Vector<T> first;
        size_t j = 0;

        //initial min/max values in case the vector contains only one triplet
        vecMin.x = source[0];
        vecMin.y = source[1];
        vecMin.z = source[2];

        vecMax.x = source[0];
        vecMax.y = source[1];
        vecMax.z = source[2];

        for (size_t i = 3; i<source.size()/3; i+=3)
        {
            vecMax.x = std::max(source[j+0], source[i+0]);
            vecMax.y = std::max(source[j+1], source[i+1]);
            vecMax.z = std::max(source[i+2], source[i+2]);

            vecMin.x = std::min(source[j+0], source[i+0]);
            vecMin.y = std::min(source[j+1], source[i+1]);
            vecMin.z = std::min(source[i+2], source[i+2]);
            
            j = i;
        }
        return true;
    }
}
#endif
//
//  geomTypedefs.h
//  
//
//  Created by oleg zilberman on 5/2/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
#ifndef __GEOMTYPEDEFS_H_
#define __GEOMTYPEDEFS_H_
namespace geomUtils
{
    typedef std::vector<float> tVectorOfFloats;
    typedef std::vector<unsigned short> tVectorOfShorts;
    typedef std::vector<GLuint> tVectorOfUints;
    struct Frustum
    {
        float zNear;
        float zFar;
        float aspect;
        float fov;
        tVectorOfFloats projection;
    };
    
    template<typename T>
    struct Point
    {
        T x, y, z;  
    };
    typedef Point<GLfloat> tPointF;
    typedef Point<double> tPointD;
    typedef Point<GLint> tPointI;

    template<typename T>
    struct Vector
    {
        T x, y, z;
    };
    typedef Vector<GLfloat> tVectorF;
    typedef Vector<GLint> tVectorI;
    typedef Vector<double> tVectorD;

    template<typename T>
    struct Rectangle
    {
        Point<T> ul, lr;
    };
    typedef Rectangle<GLfloat> tRectagleF;
    typedef Rectangle<double> tRectagleD;
    typedef Rectangle<GLint> tRectangleI;

    typedef std::vector<Point<GLfloat> > tVectorOfPointsF;
    typedef std::vector<Point<GLint> > tVectorOfPointsI;
    const static double PI = 3.1415926535897932384626433832795028841971693993751058209;
#define DECL_VEC(x,y,z) \
    geomUtils::tVectorOfFloats vec(3); \
    vec[0]=x;vec[1]=y;vec[2]=z; \
   
};
#endif

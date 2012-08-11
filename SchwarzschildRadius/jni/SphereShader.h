#ifndef _SPHERESHADER_H_
#define _SPHERESHADER_H_
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <vector>
#include "ShaderUtils.h"
#include "ShaderObjectInterface.h"
#include "ESShapes.h"

namespace SphereShader{
    static const char gVertexShader[] = 
    "uniform mat4 mvp_matrix;\n"
    "attribute vec4 a_GeometryData;\n"
    "attribute vec4 vColor;\n"
    "varying vec4 v_color;\n"
    "void main() {\n"
    "  gl_Position = mvp_matrix * a_GeometryData;\n"
    "  v_color = vColor;\n"
    "}\n";
    
    static const char gFragmentShader[] = 
    "precision mediump float;\n"
    "varying vec4 v_color;\n"
    "void main() {\n"
    "   gl_FragColor = vec4(0.5, 0.0, 0.25, 1.0);\n"
    "}\n";
    
};

class SimpleSphereShader : public ShaderObjectInterface
{
    GLuint m_programID;
    GLuint m_GeometryData;
    GLuint m_Color;
    geomUtils::geomData m_Sphere;
    GLuint m_indexCount;
    
public:
    SimpleSphereShader() : m_programID(0), m_Color(0), m_indexCount(0)
    {
        std::vector<GLuint> shaderList;
        m_indexCount = genSphere(50, 0.5, m_Sphere); //create a programmatic sphere.
        
        shaderList.push_back(shaderUtils::CreateShader(GL_VERTEX_SHADER, SphereShader::gVertexShader));
        shaderList.push_back(shaderUtils::CreateShader(GL_FRAGMENT_SHADER, SphereShader::gFragmentShader));
        m_programID = shaderUtils::CreateProgram(shaderList);
        if (m_programID == 0) {
            throw "Failure creating shader program";
        }
        std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
        
        m_GeometryData = glGetAttribLocation(m_programID, "a_GeometryData");
        m_Color = glGetAttribLocation(m_programID, "vColor");
    }
    ~SimpleSphereShader()
    {
    }
    bool operator()(const geomUtils::tVectorOfFloats& matrix) const
    {
        float viewport[4];
        glGetFloatv(GL_VIEWPORT, viewport);
        float color[4];
        color[shaderUtils::RED]=   1.0f;
        color[shaderUtils::GREEN]= 0.0f;
        color[shaderUtils::BLUE]=  0.0f;
        color[shaderUtils::ALPHA]= 0.0f;
        
        glUseProgram(getProgramID());
        glVertexAttribPointer(m_GeometryData, 3, GL_FLOAT, GL_FALSE, 0, &m_Sphere.mVertices[0]);
        glVertexAttribPointer(m_Color, 4, GL_FLOAT, GL_FALSE, 0, color);
        GLint mvp_matrix_location = glGetUniformLocation(getProgramID(), "mvp_matrix");
        if (mvp_matrix_location == -1)
            LOGI("SimpleSphereShader operator()", "=-=-=-=-= glGetUniformLocation failed");
        else
            glUniformMatrix4fv(mvp_matrix_location, 1, GL_FALSE, &matrix[0]);
        
        glEnableVertexAttribArray(m_GeometryData);
        glEnableVertexAttribArray(m_Color);
        glDrawElements(GL_LINES, m_indexCount, GL_UNSIGNED_SHORT, &m_Sphere.mIndices[0]);
        
        glDisableVertexAttribArray(m_GeometryData);
        glDisableVertexAttribArray(m_Color);
        glUseProgram(0);
        
    }
    GLuint getProgramID() const
    {
        return m_programID;
    }
    const geomUtils::tVectorOfFloats& getVertexes() const { return m_Sphere.mVertices;}
    const geomUtils::tVectorOfShorts& getIndexes() const {return m_Sphere.mIndices;}
    const char* getVertexAttribute() const{return "a_GeometryData";}

    void getMinMax(geomUtils::tVectorF& vecMin, geomUtils::tVectorF& vecMax) const
    {
        computeMinMax3DLinear(m_Sphere.mVertices, vecMin, vecMax);
    }
};
#endif
#ifndef _SIMPLETRIANGLESHADER_H_
#define _SIMPLETRIANGLESHADER_H_
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <vector>
#include "ShaderUtils.h"
#include "ShaderObjectInterface.h"
namespace SimpleShader{
    static const char gVertexShader[] = 
    "uniform mat4 mvp_matrix;\n"
    "attribute vec4 vPosition;\n"
    "attribute vec4 vColor;\n"
    "varying vec4  v_color;\n"
    "void main() {\n"
    "  gl_Position = mvp_matrix * vPosition;\n"
    "  v_color = vColor;\n"
    "}\n";

    static const char gFragmentShader[] = 
    "precision mediump float;\n"
    "varying vec4  v_color;\n"
    "void main() {\n"
    "   gl_FragColor = v_color;\n"
    "}\n";
    const GLfloat gTriangleVertices[] = 
    { 
        0.0f,    0.7f, 0.0f, 1.0f,
        0.7f, -0.366f, 0.0f, 1.0f,
        -0.7f, -0.366f, 0.0f, 1.0f,
        1.0f,    0.0f, 0.0f, 1.0f,
        0.0f,    1.0f, 0.0f, 1.0f,
        0.0f,    0.0f, 1.0f, 1.0f,
    };
};

class SimpleTriangleShader : public ShaderObjectInterface
{
    GLuint m_programID;
    GLuint m_PositionHandle;
    GLuint m_Color;
    geomUtils::geomData m_Triangle;
    
public:
    SimpleTriangleShader() : m_programID(0), m_Color(0)
    {
        std::vector<GLuint> shaderList;
        geomUtils::genTriangle(m_Triangle);
        
        shaderList.push_back(shaderUtils::CreateShader(GL_VERTEX_SHADER, SimpleShader::gVertexShader  ));
        shaderList.push_back(shaderUtils::CreateShader(GL_FRAGMENT_SHADER, SimpleShader::gFragmentShader));
        m_programID = shaderUtils::CreateProgram(shaderList);
        if (m_programID == 0) {
            throw "Failure creating shader program";
        }
        std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
        m_PositionHandle = glGetAttribLocation(m_programID, "vPosition");
        m_Color = glGetAttribLocation(m_programID, "vColor");
    }
    ~SimpleTriangleShader()
    {
    }
    bool operator()(const geomUtils::tVectorOfFloats& matrix) const
    {
        float viewport[4];
        glUseProgram(getProgramID());
        glGetFloatv(GL_VIEWPORT, viewport);
        glVertexAttribPointer(m_PositionHandle, 4, GL_FLOAT, GL_FALSE, 0, &m_Triangle.mVertices[0]);
        glVertexAttribPointer(m_Color, 4, GL_FLOAT, GL_FALSE, 0, &SimpleShader::gTriangleVertices[12]);
        GLint mvp_matrix_location = glGetUniformLocation(getProgramID(), "mvp_matrix");
        if (mvp_matrix_location == -1)
            LOGI("SimpleTriangleShader operator()", "=-=-=-=-= glGetUniformLocation failed");
        else
            glUniformMatrix4fv(mvp_matrix_location, 1, GL_FALSE, &matrix[0]);

        glEnableVertexAttribArray(m_PositionHandle);
        glEnableVertexAttribArray(m_Color);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableVertexAttribArray(m_PositionHandle);
        glEnableVertexAttribArray(m_Color);
        glUseProgram(0);
        
    }
    GLuint getProgramID() const {return m_programID;}
    const geomUtils::tVectorOfFloats& getVertexes() const {return m_Triangle.mVertices;}
    const geomUtils::tVectorOfShorts& getIndexes() const {return m_Triangle.mIndices;}
    const char* getVertexAttribute() const {return "vPosition";}   
    void getMinMax(geomUtils::tVectorF& vecMin, geomUtils::tVectorF& vecMax) const
    {
        computeMinMax3DLinear(m_Triangle.mVertices, vecMin, vecMax);
    }

    
};
#endif
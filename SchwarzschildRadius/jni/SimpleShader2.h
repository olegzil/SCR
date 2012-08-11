#ifndef _SIMPLESHADER2_H_
#define _SIMPLESHADER2_H_
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <vector>
#include "ShaderUtils.h"
#include "ShaderObjectInterface.h"

namespace SimpleShader2{
    static const char gVertexShader[] = 
    "uniform mat4 mvp_matrix;\n"
    "attribute vec4 vPosition;\n"
    "attribute float vHeight;\n"
    "attribute float vWidth;\n"
    "varying float v_height;\n"
    "varying float v_width;\n"
    "void main() {\n"
    "  gl_Position = mvp_matrix * vPosition;\n"
    "   v_height = vHeight;\n"
    "   v_width = vWidth;\n"
    "}\n";
    
    static const char gFragmentShader[] = 
    "precision mediump float;\n"
    "varying float v_height;\n"
    "varying float v_width;\n"
    "vec4 foo1()\n"
    "{\n"
    "    float lerpValue = gl_FragCoord.x / v_width;\n"
    "    return mix(vec4(1.0, 1.0, 1.0, 1.0),\n"
    "                      vec4(0.1, 0.1, 0.1, 0.1), lerpValue);\n"
    "}\n"
    "void main() {\n"
    "   gl_FragColor = foo1();\n"
    "}\n";
    const float vertexData[] = 
    {
        0.0f, 1.0f, 0.0f, 1.0f, 
        -1.0f, -1.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 1.0f 
    };
};

class SimpleTriangleShader2 : public ShaderObjectInterface
{
    GLuint m_programID;
    GLuint m_PositionHandle;
    GLuint m_WidthHandle;
    GLuint m_HeightHandle;

public:
    SimpleTriangleShader2() : m_programID(0)
    {
        std::vector<GLuint> shaderList;
        shaderList.push_back(shaderUtils::CreateShader(GL_VERTEX_SHADER, SimpleShader2::gVertexShader));
        shaderList.push_back(shaderUtils::CreateShader(GL_FRAGMENT_SHADER, SimpleShader2::gFragmentShader));
        m_programID = shaderUtils::CreateProgram(shaderList);
        if (m_programID == 0) {
            throw "Failure creating shader program";
        }
        std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
        
        m_PositionHandle = glGetAttribLocation(m_programID, "vPosition");
        m_WidthHandle = glGetAttribLocation(m_programID, "vWidth");
        m_HeightHandle = glGetAttribLocation(m_programID, "vHeight");
    }
    bool operator()(const geomUtils::tVectorOfFloats& matrix) const
    {
        float viewport[4];
        glUseProgram(getProgramID());
        glGetFloatv(GL_VIEWPORT, viewport);
        glVertexAttribPointer(m_PositionHandle, 4, GL_FLOAT, GL_FALSE, 0, SimpleShader2::vertexData);
        glVertexAttribPointer(m_WidthHandle, 1, GL_FLOAT, GL_FALSE, 0, &viewport[2]);
        glVertexAttribPointer(m_HeightHandle, 1, GL_FLOAT, GL_FALSE, 0, &viewport[3]);
        GLint mvp_matrix_location = glGetUniformLocation(getProgramID(), "mvp_matrix");
        if (mvp_matrix_location == -1)
            LOGI("SimpleTriangleShader2 operator()", "=-=-=-=-= glGetUniformLocation failed");
        else
            glUniformMatrix4fv(mvp_matrix_location, 1, GL_FALSE, &matrix[0]);
        
        glEnableVertexAttribArray(m_PositionHandle);
        glEnableVertexAttribArray(m_WidthHandle);
        glEnableVertexAttribArray(m_HeightHandle);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glUseProgram(0);
        glDisableVertexAttribArray(m_PositionHandle);
        glDisableVertexAttribArray(m_WidthHandle);
        glDisableVertexAttribArray(m_HeightHandle);
        
    }
    GLuint getProgramID() const
    {
        return m_programID;
    }
};
#endif
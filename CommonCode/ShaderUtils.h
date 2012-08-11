#ifndef _SHADERUTILS_H_
#define _SHADERUTILS_H_
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <string>
#include <vector>

namespace shaderUtils
{
    enum {
        RED,
        GREEN,
        BLUE,
        ALPHA,
    };
    
    template <typename T>
    struct scoped_array
    {
        T * data;
        scoped_array(T *d) : data(d){}
        ~scoped_array()
        {
            delete[] data;
        }
        T* operator-> ()
        {
            return data;
        }
        T* get(){return data;}
    };
    static const int MAX_ERROR_BUFFER_LEN = 4096;
    inline
    void DisplayShaderError(GLuint shader, GLenum eShaderType, const char* TAG)
    {
        GLint infoLogLength;
        GLsizei buffLen=0;
        GLchar *strInfoLog = new GLchar[MAX_ERROR_BUFFER_LEN];
        memset(strInfoLog, 0, MAX_ERROR_BUFFER_LEN);
        glGetShaderInfoLog(shader, MAX_ERROR_BUFFER_LEN, &buffLen, strInfoLog);
        const char *strShaderType = NULL;
        switch(eShaderType)
        {
            case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
            case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
        }
        
        LOGI(TAG, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
        LOGI(TAG, "Error buffer length is %d\nErrors: %s\n", infoLogLength, strInfoLog);
        delete[] strInfoLog;
    }
    
    inline
    GLuint CreateShader(GLenum eShaderType, const std::string &strShaderFile)
    {
        GLuint shader = glCreateShader(eShaderType);
        const char *strFileData = strShaderFile.c_str();
        glShaderSource(shader, 1, &strFileData, NULL);
        
        glCompileShader(shader);
        
        GLint status;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        if (status == GL_FALSE){
            DisplayShaderError(shader, eShaderType, "CreateShader");
            shader = 0;
        }
        
        return shader;
    }
    
    inline
    GLuint CreateProgram(const std::vector<GLuint> &shaderList)
    {
        GLuint program = glCreateProgram();
        
        for(size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
            glAttachShader(program, shaderList[iLoop]);
        
        glLinkProgram(program);
        
        GLint status;
        glGetProgramiv (program, GL_LINK_STATUS, &status);
        if (status == GL_FALSE)
        {
            GLint infoLogLength;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
            
            GLchar *strInfoLog = new GLchar[infoLogLength + 1];
            glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
            fprintf(stderr, "Linker failure: %s\n", strInfoLog);
            delete[] strInfoLog;
        }
        
        for(size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
            glDetachShader(program, shaderList[iLoop]);
        
        return program;
    }
    

}//end namespace shaderUtils 
#endif

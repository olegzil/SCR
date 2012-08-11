#include <GLES/gl.h>
#include <GLES/glext.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <cstdio>
#include <cstdlib>
#include "tr1/memory"
#include "tr1/shared_ptr.h"
#include <cmath>
#include <algorithm>

#include "TouchInputManager.h"
#include "utils.h"
#include "ShaderUtils.h"
#include "ESShapes.h"
#include "Renderer.h"
#include "SimpleTriangleShader.h"
#include "SimpleShader2.h"
#include "SphereShader.h"
#include "geomUtils.h"
#include "JsonParser.h"

struct execute{
    const geomUtils::tVectorOfFloats& _matrix;
    execute(const geomUtils::tVectorOfFloats& matrix) : _matrix(matrix){}
    template <typename T>
    void operator()(T item)
    {
        (*item)(_matrix);
    }
};
struct modify{
    geomUtils::tVectorOfFloats& _matrix;
    modify(geomUtils::tVectorOfFloats& matrix) : _matrix(matrix){}
    template <typename T>
    void operator()(T item)
    {
        LOGD("=-=-=-=-= from modify ", "before operator call");
        (*item)(_matrix);
    }
    
};

extern "C"
{
    void printGLString(const char *name, GLenum s);
    void checkGlError(const char* op);
}
static const char* TAG="RENDERER =-=-=-=";
//TODO: add ifdef for iOS

// OpenGL ES 2.0 code


static const char* gLOG_TAG = "RENDERER";

Renderer* Renderer::gInstance = NULL;
Renderer::Renderer() 
{
    clearAllMatrixes();
}

Renderer::~Renderer()
{
}
Renderer* Renderer::Instance(){
    if (gInstance == NULL)
        gInstance = new Renderer();
    return gInstance;
}
void Renderer::DestroyInstance(){
    if (!gInstance)
        delete gInstance;
    gInstance = NULL;
}
void Renderer::InitializeVertexBuffer()
{
#if 0
    glGenBuffers(1, &frameBuffers[0]);
    
    glBindBuffer(GL_ARRAY_BUFFER, frameBuffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
}

void Renderer::clearAllMatrixes()
{
    m_model.resize(16);
    m_view.resize(16);
    m_projection.resize(16);
    m_combinedView.resize(16);
    geomUtils::makeIdentityMatrix(m_model);
    geomUtils::makeIdentityMatrix(m_view);
    geomUtils::makeIdentityMatrix(m_projection);
    geomUtils::makeIdentityMatrix(m_combinedView);
}

void Renderer::InitializeView(int w, int h)
{
    clearAllMatrixes();
    GLfloat zNear = 1.0f;
    GLfloat diam = 1000.0;
    GLfloat zFar = zNear + diam;
    GLfloat left = 0.0 - diam;
    GLfloat top = 0.0 + diam;
    GLfloat viewAngle = 45.0; 
    
    GLfloat aspect = (GLfloat) ((GLfloat)w /(GLfloat) h);
    
    m_modifiers.clear();
    const geomUtils::tVectorOfFloats::value_type ev[3]={0.0, 0.0, -3.0f}; 
    /*eye vector*/    geomUtils::tVectorOfFloats eyeVector(ev, ev+3);
    
    const geomUtils::tVectorOfFloats::value_type cv[3]={0.0, 0.0, 0.0};
    /*center vector*/geomUtils::tVectorOfFloats centerVector(cv, cv+3);
    
    const geomUtils::tVectorOfFloats::value_type uv[3]={0.0, 1.0, 0.0};
    /*up vector*/     geomUtils::tVectorOfFloats upVector(uv, uv+3); 
    geomUtils::makeIdentityMatrix(m_combinedView);
    geomUtils::makeIdentityMatrix(m_projection); //the natvie OpenGL projection matrix does not affect shaders.  Need to maintain your own.
    geomUtils::gluPerspective(viewAngle, aspect, zNear, zFar,   m_projection); //create a perspective projection
    geomUtils::gluLookAt (/*eye vector*/    eyeVector, 
                       /*center vector*/    centerVector, 
                       /*up vector*/        upVector, 
                       /*the computed view matrix*/m_projection, m_view);    //create the view matrix
    geomUtils::matrixMultiply(m_view, m_combinedView, m_combinedView); //combine the model and projection matrixes

    AffineTransformClient<geomUtils::rotationOperator>* rotation = createRotationNotifers(w, h);
    AffineTransformClient<geomUtils::cameraTranslationOperator>* translation = createTranslationNotifiers(w,h);
    
    m_frustum.zNear = zNear;
    m_frustum.zFar = zFar;
    m_frustum.aspect = aspect;
    m_frustum.fov = viewAngle;
    m_frustum.projection = m_projection;
    
    //rotation->Toggle(); //calling Toggle() toggles the feature. In this case, the feature is on.
    translation->Toggle();  //the feature is off
}

AffineTransformClient<geomUtils::rotationOperator>*  Renderer::createRotationNotifers(int w, int h)
{
    //***************** Create rotation notifier *******************************
    geomUtils::rotationOperator opr1(m_combinedView, float(w), float(h));
    AffineTransformClient<geomUtils::rotationOperator>* rotation =  new AffineTransformClient<geomUtils::rotationOperator>("rotationClient", opr1, m_combinedView); //the rotation client implements both the IModifierClient and IEventClient interfaces
    Notification::tModifierClient modifierClient(rotation); //managed IModifierClient interface
    m_modifiers.push_back(modifierClient);              //add it
    InputManager::Instance()->RegisterClient(std::tr1::dynamic_pointer_cast<Notification::IEventClient>(modifierClient));
    return rotation;
}
AffineTransformClient<geomUtils::cameraTranslationOperator>* Renderer::createTranslationNotifiers(int w, int h)
{
    //***************** Create translation notifier *******************************
    geomUtils::cameraTranslationOperator opr2(m_view, m_projection, m_combinedView, float(w), float(h));
    AffineTransformClient<geomUtils::cameraTranslationOperator>* translation =  new AffineTransformClient<geomUtils::cameraTranslationOperator>("translationClient", opr2, m_combinedView); //the rotation client implements both the IModifierClient and IEventClient interfaces
    Notification::tModifierClient modifierClient(translation); //managed IModifierClient interface
    m_modifiers.push_back(modifierClient);                     //add it
    InputManager::Instance()->RegisterClient(std::tr1::dynamic_pointer_cast<Notification::IEventClient>(modifierClient));
    return translation;
}

void Renderer::UpdateSceneGraph(const JSON::jsonChar* json, int len)
{
    m_sceneGraphManager.reset(new SceneGraphManager);
    if (m_sceneGraphManager == NULL)
    {
        LOGE(TAG, "failed to create SceneGraphManager");
        return;
    }
    if (m_sceneGraphManager->populateSceneGraph(json) ==false)
    {
        LOGE(TAG, "failed to parse scenegraph json");
        return;
    }
}

bool Renderer::InitializeShaders()
{
//    m_shaderContainer.push_back(tShaderObjectPtr(new SimpleTriangleShader()));
    m_shaderContainer.push_back(tShaderObjectPtr(new SimpleSphereShader()));
    return true;
}

bool Renderer::setupGraphics(int w, int h){
    EGLint major, minor;
    LOGD(TAG, "=-=-=-=-= from setupGraphics");
    InitializeVertexBuffer();
    InitializeView(w, h);
    
    if (!InitializeShaders()) {
        LOGE(TAG, "=-=-=-=-= Could not create program.");
        return false;
    }
    glViewport(0, 0, w, h);
}


void Renderer::renderFrame(){
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    std::for_each(m_shaderContainer.begin(), m_shaderContainer.end(), execute(m_combinedView));
    geomUtils::matrixMultiply(m_combinedView, m_model, m_combinedView);
}


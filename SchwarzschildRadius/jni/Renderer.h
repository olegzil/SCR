#ifndef _RENDERER_H_
#define _RENDERER_H_
#include <EGL/egl.h>
#include <string>
#include <vector>
#include <algorithm>
#include <tr1/shared_ptr.h>
#include "ShaderObjectInterface.h"
#include "geomUtils.h"
#include "AffineTransformClient.h"
#include "SceneGraphManager.h"

class Renderer
{
    typedef std::tr1::shared_ptr<ShaderObjectInterface> tShaderObjectPtr;
    typedef std::vector<tShaderObjectPtr> tShaderObjectList;
    typedef std::vector<Notification::tModifierClient> tModifierList;
    tShaderObjectList m_shaderContainer;
    tSceneGraphManagerPtr m_sceneGraphManager;
    geomUtils::Frustum m_frustum;
    
    geomUtils::tVectorOfFloats m_model, m_view, m_projection, m_combinedView;
    tModifierList m_modifiers;
    
    GLuint frameBuffers[2];
    EGLDisplay defaultDisplay;
    static Renderer* gInstance;
    Renderer();
    void InitializeVertexBuffer();
    bool InitializeShaders();
    void InitializeView(int, int);
    void clearAllMatrixes();
    AffineTransformClient<geomUtils::rotationOperator>* createRotationNotifers(int, int);
    AffineTransformClient<geomUtils::cameraTranslationOperator>* createTranslationNotifiers(int, int);
    
public:    
    ~Renderer();
    bool setupGraphics(int w, int h);
    void renderFrame();
    void UpdateSceneGraph(const char*, int len); 
    static Renderer* Instance();
    static void DestroyInstance();
};
#endif

#ifndef _SHADEROBJECTINTERFACE_H_
#define _SHADEROBJECTINTERFACE_H_
#include "geomUtils.h"
#ifdef __ANDROID__
#include "tr1/memory"
#include "tr1/shared_ptr.h"
#define TR1_NAMESPACE std::tr1
#else
#include <memory>
#include <boost/shared_ptr.hpp>
#define TR1_NAMESPACE boost
#define LOGD(X,...)/**/
#endif
class ShaderObjectInterface
{
public:
    virtual ~ShaderObjectInterface(){};
    virtual bool operator()(const geomUtils::tVectorOfFloats&) const = 0;
    virtual GLuint getProgramID() const = 0;
    virtual const geomUtils::tVectorOfFloats& getVertexes() const = 0;
    virtual const geomUtils::tVectorOfShorts& getIndexes() const = 0;
    virtual const char* getVertexAttribute() const = 0;
    virtual void getMinMax(geomUtils::tVectorF& vecMin, geomUtils::tVectorF& minMax) const = 0;
};
typedef TR1_NAMESPACE::shared_ptr<ShaderObjectInterface> tShaderObjectInterfacePtr;
typedef TR1_NAMESPACE::shared_ptr<const ShaderObjectInterface> tShaderObjectInterfaceConstPtr;
#endif
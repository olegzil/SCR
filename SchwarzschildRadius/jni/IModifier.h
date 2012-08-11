#ifndef _IMODIFIER_H_
#define _IMODIFIER_H_
#include "geomUtils.h"
namespace Notification
{
class IModifier
{
public:
    virtual bool operator()(geomUtils::tVectorOfFloats&) = 0;
    virtual ~IModifier(){}
};
    typedef std::tr1::shared_ptr<IModifier> tModifierClient;
}
#endif
#ifndef _IEVENTCLIENT_H_
#define _IEVENTCLIENT_H_
#include "TouchEvent.h"
namespace Notification
{
    class IEventClient
    {
    public:
    	virtual bool operator==(const IEventClient* rhs) const = 0;
    	virtual const std::string& key() const = 0;
        virtual bool operator()(const TouchEvent& te) = 0;
        virtual bool isOn() const = 0;
        virtual bool Toggle() = 0;
        virtual ~IEventClient(){}
    };
}
#endif
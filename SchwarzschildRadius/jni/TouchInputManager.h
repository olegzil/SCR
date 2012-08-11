#ifndef _TOUCHINPUTMANAGER_H_
#define _TOUCHINPUTMANAGER_H_
#include <tr1/memory>
#include <tr1/shared_ptr.h>
#include <list>
#include "IEventClient.h"
class InputManager
{
public:
    typedef std::tr1::shared_ptr<Notification::IEventClient> tEventClient;
    typedef std::tr1::weak_ptr<Notification::IEventClient> tEventClientRef;

private:
    typedef std::list<tEventClient> tEventClientList;
    typedef std::list<tEventClient>::iterator tEventClientIterator;
    tEventClientList mEventClientList;
    struct eventComparator
    {
        const tEventClientRef target;
        eventComparator(const tEventClient& t) : target(t){}
        template<typename T>
        bool operator()(const T& item) const
        {
            if (item->key() == target.lock()->key())
                return true;
            return false;
        }
    };
private:
    InputManager() {}
    static InputManager* mInstance;
public:
    ~InputManager(){}
    bool RegisterClient(tEventClient);
    bool UnregisterClient(tEventClient);
    void EventLoop(const Notification::TouchEvent& event);
    static InputManager* Instance(){
        if (mInstance)
            return mInstance;
        mInstance = new InputManager();
        return mInstance;
    }
    static void DestroyInstance(){
        if (!mInstance)
            delete mInstance;
        mInstance = NULL;
    }
};
#endif

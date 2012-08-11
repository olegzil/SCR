#include <cstddef>
#include "algorithm"
#include "utils.h"
#include "TouchInputManager.h"
InputManager* InputManager::mInstance = NULL;
bool InputManager::RegisterClient(tEventClient client)
{
    tEventClientIterator target =  std::find_if(mEventClientList.begin(), mEventClientList.end(), eventComparator(client));
    if (target != mEventClientList.end())
        return false;
    mEventClientList.push_back(client);
    return true;
}
bool InputManager::UnregisterClient(tEventClient client)
{
    tEventClientIterator target =  std::find_if(mEventClientList.begin(), mEventClientList.end(), eventComparator(client));
    if (target == mEventClientList.end())
        return false;
    mEventClientList.erase(target);
    return true;
}

void InputManager::EventLoop(const Notification::TouchEvent& event)
{
    for (tEventClientIterator i = mEventClientList.begin(); i != mEventClientList.end(); ++i)
        if((*i)->isOn())
            (**i)(event);
}
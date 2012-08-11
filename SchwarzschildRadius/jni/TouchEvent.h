#ifndef _TOUCHEVENT_H_
#define _TOUCHEVENT_H_
namespace Notification
{
    typedef enum ActionType
    {
            ACTION_MOVE = 1,
            ACTION_UP,
            ACTION_POINTER_UP,
            ACTION_POINTER_DOWN,
            ACTION_DOWN,
            ACTION_UNKNOWN=255
    };
    struct TouchEvent
    {
        float x, y;
        unsigned long time;
        ActionType action;
        int index;
    };
}
#endif
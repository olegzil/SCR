#ifndef _TRANSLATION_CLIENT_H_
#define _TRANSLATION_CLIENT_H_
#include "ITransaltionClient.h"
#include "IEventClient.h"

class TranslationClient : public ITransaltionClient, public IEventClient
{
    bool m_on;
public:
    TranslationClient() : m_on(true) {}
    bool isOn() const {return m_on;}
    bool Toggle()
    {
        bool temp = m_on;
        m_on = !m_on;
        return temp;
    }
    bool operator()(const Notification::TouchEvent& te);
    bool operator()(geomUtils::tVectorOfFloats&);
}
#endif
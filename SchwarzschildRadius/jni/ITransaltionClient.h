#ifndef _ITRANSLATION_CLIENT_H_
#define _ITRANSLATION_CLIENT_H_
class ITranslationClient
{
public:
    virtual ~ITranslationClient(){}
    virtual bool isOn() const = 0;
    virtual bool Toggle() = 0;
    virtual bool operator()(const geomUtils::tVectorOfFloats& vec) = 0;
}
#endif
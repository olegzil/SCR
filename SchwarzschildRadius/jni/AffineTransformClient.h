#ifndef _AFFINETRANSFORMCLIENT_H_
#define _AFFINETRANSFORMCLIENT_H_
#include "tr1/memory"
#include "tr1/shared_ptr.h"
#include <string>
#include <vector>
#include <utility>
#include <cmath>
#include "utils.h"

#include "IEventClient.h"
#include "IModifier.h"

static const float gMaxPointSample=3;
static const float gMinDiff = 0.000001f;

template<typename T>
class AffineTransformClient : public Notification::IEventClient, public Notification::IModifier
{
    typedef std::vector<std::pair<float, float> > tPointVector;
    T m_operator;
    geomUtils::tVectorOfFloats& m_matrix;
    tPointVector m_points;
    geomUtils::Point<GLfloat> m_head, m_tail;
    int m_axis;
    bool m_on;
    float m_delta;
    const std::string m_key;

    geomUtils::tVectorOfFloats m_rotationAngle;
    geomUtils::tVectorOfFloats m_rate;
    int computeDirection()
    {
        float deltaX = 0.f;
        float deltaY = 0.f;
        int directionX, directionY;
        deltaX = 0;
        for(size_t i=0; i<m_points.size()-1; i++)
            deltaX = std::max( std::abs(m_points[i].first - m_points[i+1].first), deltaX);
            
        for(size_t i=0; i<m_points.size()-1; i++)
            deltaY = std::max( std::abs(m_points[i].second - m_points[i+1].second), deltaY);
        
        int last = m_points.size() - 1;
                            
        directionX = (m_points[0].first  - m_points[last].first)  > 0 ? 1 : -1;
        directionY = (m_points[0].second - m_points[last].second) > 0 ? 1 : -1;

        m_delta=deltaY*directionY;
        if ( (std::abs(deltaY) > 0 || std::abs(deltaX > 0)) && (std::abs(deltaX-deltaY) <= gMinDiff) )
        {
            //if both x and you are changing at the same rate, rotate around z-axis
            m_axis = 3;
            return m_axis;
        }
        if (std::abs(deltaX-deltaY) <= gMinDiff) //neither x or y is changing
        {
            m_axis = 0;
            return m_axis;
        }
        
        if (deltaX > deltaY) //x is changing faster then y
        {
            m_axis = 1;
            m_delta=deltaX*directionX;
            return m_axis;
        }
        m_axis = 2;
        return m_axis;            //y is changing faster then x
    }
    
    bool onMove(const Notification::TouchEvent& te)
    {
        bool retVal = false;
        if (m_points.size() < gMaxPointSample)
            m_points.push_back(std::make_pair(te.x, te.y));
        else{
            computeDirection();
            geomUtils::tVectorOfFloats::iterator first, last;
            m_head.x = m_points[0].first;
            m_head.y = m_points[0].second;
            m_tail.x = m_points[m_points.size()-1].first;
            m_tail.y = m_points[m_points.size()-1].second;
            m_points.clear();
            m_points.push_back(std::make_pair(te.x, te.y));
            retVal = true;
        }
        return retVal;
    }
    
    int getRotationAxis() const {return m_axis;}
public:

    const std::string& key() const
    {
        return m_key;
    }

    bool operator==(const Notification::IEventClient* rhs) const
    {
        return m_key == rhs->key();
    }
    AffineTransformClient(const std::string& key, T& action, geomUtils::tVectorOfFloats& matrix) : m_axis(-1), m_on(true), m_delta(0.0f), m_operator(action), m_matrix(matrix), m_key(key)
  
    {
        m_points.resize(gMaxPointSample);
        std::fill(m_points.begin(), m_points.end(), std::make_pair(0., 0));
    }
    bool isOn() const
    {
        return m_on;
    }
    bool Toggle()
    {
        bool temp = m_on;
        m_on = !m_on;
        return temp;
    }
    //called by the Notification Manager to update state
    bool operator()(const Notification::TouchEvent& te)
    {
        if (te.action != Notification::ACTION_MOVE || te.index != 0) //we're moving only one finger on the screen
            return false;
        
        bool retVal = onMove(te); //sample data
        if (retVal){
            (*this)(m_matrix);
        }
        return retVal;
    }
    bool operator()(geomUtils::tVectorOfFloats& matrix)
    {
        bool retVal = true;
        switch (getRotationAxis()) 
        {
            case 1: //x is rotating
            {
                DECL_VEC(1.0, 0.0, 0.0)
                m_operator(vec, getRotationAxis(), m_head, m_tail, m_delta);
                break;
            }
                
            case 2: //y is rotating
            {
                DECL_VEC(0.0, 1.0, 0.0)
                m_operator(vec, getRotationAxis(), m_head, m_tail, m_delta);
                break;
            }
                
            case 3: //z is rotating
            {
                DECL_VEC(0.0, 0.0, 1.0)
                m_operator(vec, getRotationAxis(), m_head, m_tail, m_delta);
                break;
            }
                
            default:
                retVal = false;
                break;
        }
        return retVal;
    }
};
#endif
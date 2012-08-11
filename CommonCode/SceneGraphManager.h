#ifndef _SCENEGRAPH_MANAGER_H_
#define _SCENEGRAPH_MANAGER_H_
#include <set>
#include <functional>
#include <iostream>
#include "ShaderObjectInterface.h"
#include "SceneGraphNode.h"
#include "JsonParser.h"

#include "JsonParser.h"
struct SceneGraphBuilder{
    const std::string& _qualifiedName;
    typedef std::vector< std::string > split_vector_type;
    SceneGraphBuilder(const std::string& name) : _qualifiedName(name){}

    template<typename T1, typename T2>
    bool operator()(T1& name, T2& hashTable)
    {
        if ( strcasecmp(name.c_str(), _qualifiedName.c_str()) == 0)
        {
            JSON::tObjectType o;
            JSON::JsonObject& item = *(hashTable[name]); //found an array of objects, i.e. tJsonObjectPtr contains a pointer to an tJsonArrayPtr
            JSON::tJsonObjectArrayPtr objects = boost::any_cast<JSON::tJsonObjectArrayPtr>(item._value);
            JSON::tParsedJsonPtr obj;
            for (JSON::tJsonObjectArray::iterator i=objects->begin(); i!=objects->end(); ++i) //iterate through the entire array
            {
                switch(i->_type)
                {
                    case JSON::eObject:                                                                             //we're only interested in eObject types
                        obj = boost::any_cast<JSON::tParsedJsonPtr>(i->_value);                                     //each item in this array is an any-type object that is really a tParsedJsonPtr type. So cast it appropriately
                        for (JSON::tParsedJsonPtr::element_type::iterator i=obj->begin(); i!=obj->end(); ++i)         //the tParsedJsonPtr is a pointer to a hash table. Traverse the hash table
                        {
                            std::string key(i->first);                                                              //this is the key into the hash table
                            std::string value;
                            if (i->second->_type == JSON::eString)                                                  //if this object is a string, process it.
                            {
                                JSON::tJsonObjectPtr p = (*obj)[key];                                               //a string is stored as an JsonObjectPtr 
                                JSON::tJsonStringPtr str = boost::any_cast<JSON::tJsonStringPtr>(p->_value);        //convert the obect to a string pointer
                                std::cout << " the value at " << i->first << " is " << *str << std::endl;           //finaly display the string.
                            }
                            else if (i->second->_type == JSON::eNumber)
                            {
                                JSON::tJsonObjectPtr p = (*obj)[key];                                                   //a string is stored as an JsonObjectPtr 
                                JSON::tJsonObjectPtr numberObject = boost::any_cast<JSON::tJsonObjectPtr>(p->_value);   //convert the obect to a string pointer
                                if (numberObject->_type == JSON::eFloat)
                                {
                                    float number = boost::any_cast<float>(numberObject->_value);
                                    std::cout << " the value with key=" << i->first << " is " << number << std::endl;           //finaly display the string.
                                }
                                else if (numberObject->_type == JSON::eDecimal)
                                {
                                    int number = boost::any_cast<int>(numberObject->_value);
                                    std::cout << " the value with key=" << i->first << " is " << number << std::endl;           //finaly display the string.
                                }
                            }
                        }
                        break;
                }
            }
            return true;
        }
        return false;
    }
};

class SceneGraphManager
{	
	template <typename T> struct less : std::binary_function <T,T,bool> 
	{
	  bool operator() (const T& x, const T& y) const
	    {
	    	return *x < *y;
	    }
	};
	struct JsonParserCallback{
	    const std::string& _name;
	    JsonParserCallback(const std::string& name) : _name(name){}
	    template<typename T1, typename T2>
	    bool operator()(T1& name, T2& hashTable)
	    {
	        if ( strcasecmp(name.c_str(), _name.c_str()) == 0)
	        {
	            JSON::JsonObject& item = *(hashTable[name]);
	            return false;
	        }
	        return true;
	    }
	};
	typedef std::set<geomUtils::tSceneGraphNodePtr, less<std::string> > tSceneGraph;
	tSceneGraph _sceneGraph;

public:
	SceneGraphManager()
	{

	}
	bool populateSceneGraph(JSON::jsonString jsonData)
	{
		SceneGraphBuilder callback("SceneGraph");
    	JSON::tParsedJsonPtr p = JSON::ParseJson(jsonData, callback);
    	return p == NULL ? false : true;
	}
};
typedef TR1_NAMESPACE::shared_ptr<SceneGraphManager> tSceneGraphManagerPtr;
typedef TR1_NAMESPACE::shared_ptr<const SceneGraphManager> tSceneGraphManagerConstPtr;
#endif
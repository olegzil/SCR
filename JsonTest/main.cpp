//
//  main.cpp
//  JsonTest
//
//  Created by oleg zilberman on 5/17/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;
using namespace boost;

#include "JsonParser.h"
struct Callback{
    const std::string& _qualifiedName;
    typedef vector< string > split_vector_type;
    Callback(const std::string& name) : _qualifiedName(name)
    {
        split_vector_type SplitVec; // #2: Search for tokens
        split( SplitVec, _qualifiedName, is_any_of(".[]"), token_compress_on ); 
        for (split_vector_type::iterator i=SplitVec.begin(); i!=SplitVec.end(); ++i)
        {
            cout << " value = " << *i << "\n";
        }
    }
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
                        for (JSON::tParsedJsonPtr::value_type::iterator i=obj->begin(); i!=obj->end(); ++i)         //the tParsedJsonPtr is a pointer to a hash table. Traverse the hash table
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
int main(int argc, const char * argv[])
{
    char fileName[256];
    std::cout << "Input file name ";
    std::cin >> fileName;
    ifstream jsonFile;
    jsonFile.open(fileName, std::ios::out | std::ios::binary);

    if (jsonFile.is_open() == false){
        printf("unable to upen file for reading %s", fileName);
        exit(1);
    }
    JSON::jsonString jsonData;
    char c;
    while (!jsonFile.eof()) 
    {
        jsonFile.get(c);
        jsonData.push_back(c);
    }
    int index = 0;
    Callback callback("objects");
    JSON::tParsedJsonPtr p = JSON::ParseJson(jsonData, callback);
    if (p == NULL)
        std::cout << "null pointer returned" << "\n";
    return 0;
}


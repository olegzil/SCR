#ifndef _JSON_PARSER_H_
#define _JSON_PARSER_H_
#ifdef __ANDROID__
#include "tr1/memory"
#include "tr1/shared_ptr.h"
#include <boost/any.hpp>
#include <boost/variant.hpp>
#include <cmath>
#include <algorithm>
#include <tr1/unordered_map>
#include <tr1/unordered_set>
#include <string>
#define TR1_NAMESPACE std::tr1
#else
#include <memory>
#include <boost/shared_ptr.hpp>
#include <boost/any.hpp>
#include <boost/variant.hpp>
#include <cmath>
#include <algorithm>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>
#include <string>

#define TR1_NAMESPACE boost
#define LOGD(X,...)/**/
#endif

namespace JSON
{
	typedef char jsonChar;
	typedef std::string jsonString;
	typedef enum tJsonTypes
	{
		eOpenCurly =0xF,
		eCloseCurly,
		eOpenBracket,
		eCloseBracket,
		eColon,
		eQuote,
		eComma,
		eDigit,
		ePlus,
		ePeriod,
		eMinus,
		eExponent,
		eAlpha,
		eBooleanTrue,
		eBooleanFalse,
		eComent,
		eInvalid
	};
	typedef enum tObjectType
	{
		eObject,
		eArray,
		eFloat,
		eDecimal,
		eNumber,
		eString,
		eBool, 
		eNull
	};
	struct JsonObject; //foward declaration
	typedef TR1_NAMESPACE::shared_ptr<JsonObject> tJsonObjectPtr;
	typedef TR1_NAMESPACE::unordered_map< jsonString, tJsonObjectPtr> tHashTable;
	typedef TR1_NAMESPACE::unordered_set<jsonChar> tValidValues;

	typedef TR1_NAMESPACE::shared_ptr<tHashTable> tParsedJsonPtr;
	typedef std::vector<JsonObject> tJsonObjectArray;
	typedef TR1_NAMESPACE::shared_ptr<tJsonObjectArray> tJsonObjectArrayPtr;
	typedef TR1_NAMESPACE::shared_ptr<const tJsonObjectArray> tJsonObjectArrayConstPtr;
	typedef TR1_NAMESPACE::shared_ptr<jsonString> tJsonStringPtr;
	typedef boost::any tValueType;

	struct JsonObject
	{
		tObjectType _type;
		tValueType _value;
	};
	struct Parser
	{
		bool _terminate;
		tValidValues m_validValues;

		Parser() : _terminate(false)
		{
			for (char c=32; c<127; ++c)
				m_validValues.insert(c);
		}
		inline
		bool isValidValue(jsonChar c)
		{
			return m_validValues.find(c) == m_validValues.end() ? false : true;
		}
		inline
		bool SkipComent(const jsonString& json, int& index)
		{
			jsonChar c;
			for (; index<json.size(); index++)
			{
				c = json[index];
				if (c == 0xA || c == 0xD)
					break;
			}

			//skip the remaining white-space
			while (json.size() > index)
				switch (json[index])
				{
					case 0x20:
					case 0x09:
					case 0xA:
					case 0xD: //skip white space.
						++index;
						break;

					default:
						return true;
				}
			return false;
		}
		/*
			skips all white space until 
		*/
		inline bool SkipWhiteSpace(const jsonString& json, int& index)
		{
			while (true && json.size() > index)
				switch(json[index])
				{
					case 0x20:
					case 0x09:
					case 0xA:
					case 0xD: //skip white space.
						++index;
						break;

					default:
						if (json[index] == '/' && json[index+1] == '/')
							return SkipComent(json, index);
						return true;
				}
				return false; //done
		}

		bool isValidTerminator(jsonChar c, const jsonString& json, int index)
		{
			switch(c)
			{
				case ',':
				case ']':
				case '}':
				return true;
			}
			if (c == '/' && json[index+1] == '/') //test for comment
				return true;
			return false;
		}

		tJsonTypes testForBoolean(const jsonString& json, int& index)
		{
			if (json[index] != 't' && json[index] != 'f') //this has to be a true/false token. Lower case only.
				return eInvalid;
			jsonChar c = json[index];
			if (json[index+0] == 't' &&
				json[index+1] == 'r' &&
				json[index+2] == 'u' &&
				json[index+3] == 'e' && 
				isValidTerminator(json[index+4], json, index+4))
				{
					index += 4;
					return eBooleanTrue;
				}

			if (json[index+0] == 'f' &&
				json[index+1] == 'a' &&
				json[index+2] == 'l' &&
				json[index+3] == 's' && 
				json[index+4] == 'e' && 
				isValidTerminator(json[index+5], json, index+5))
				{
					index += 5;
					return eBooleanFalse;
				}
			return eInvalid;
		}

		bool testForNumber(const jsonString& json, int index)
		{
			//we are here because json[index] == '-' or '+'
			jsonChar c = json[index+1];
			if (c-0x30 >= 0 && c-0x30 <= 9)
				return true;
			return false;
		}

		tJsonTypes LookAhead(const jsonString& json, int index)
		{
			if (!SkipWhiteSpace(json, index))
				return eInvalid;
			tJsonTypes retVal = eInvalid;
			jsonChar c = json[index];

			//detect none-whitespace character
			switch(c)
			{
				case '{':
					retVal = eOpenCurly;
					break;

				case '}':
					retVal = eCloseCurly;
					break;

				case '[':
					retVal = eOpenBracket;
					break;

				case ']':
					retVal = eCloseBracket;
					break;

				case ',':
					retVal = eComma;
					break;

				case '"':
					retVal = eQuote;
					break;

				case ':':
					retVal = eColon;
					break;

				case 't':
				case 'f':
					retVal = testForBoolean(json, index);
					break;
				case '-':
				case '+':
					if(testForNumber(json, index))
						retVal = eDigit;
					else
						retVal = eAlpha;
					break;

				default:
					if (c-0x30 >= 0 && c-0x30 <= 9)
						retVal = eDigit;
					else
						retVal = eAlpha;
					break;
			}
			return retVal;
		}
		/*
			first non-white space character or end of buffer
			if end-of-buffer, return eInvalid
			othewise locate controling character, skip it and return find status.
		*/
		tJsonTypes NextToken(const jsonString& json, int& index)
		{
			if (!SkipWhiteSpace(json, index))
				return eInvalid;
			tJsonTypes retVal = eInvalid;
			jsonChar c = json[index];
			switch(c)
			{
				case '{':
					retVal = eOpenCurly;
					++index;
					break;

				case '}':
					retVal = eCloseCurly;
					++index;
					break;

				case '[':
					retVal = eOpenBracket;
					++index;
					break;

				case ']':
					retVal = eCloseBracket;
					++index;
					break;

				case ',':
					retVal = eComma;
					++index;
					break;

				case '"':
					retVal = eQuote;
					++index;
					break;

				case ':':
					retVal = eColon;
					++index;
					break;

				case 't':
				case 'f':
					retVal = testForBoolean(json, index);
					break;

				default:
					if (c-0x30 >= 0 && c-0x30 <= 9)
						retVal = eDigit;
					else if (c == '/' && json[index+1] == '/')
						retVal = eComent;
					else if (isValidValue(c))
						retVal = eAlpha;
					else
						retVal = eInvalid;
					break;
			}
			return retVal;
		}

		inline tJsonStringPtr ParseString(const jsonString& json, int& index)
		{
			tJsonStringPtr retVal((jsonString*)NULL);
			if (!SkipWhiteSpace(json, index)){
				return retVal; //empty string
			}			
			jsonChar c = json[index];

			NextToken(json, index); //skip leading qoute
			retVal.reset(new jsonString);
			while (json[index] != '"' && index < json.size())
				retVal->push_back(json[index++]);

			if (json[index] != '"') 
				return tJsonStringPtr();
			retVal->push_back(0);//terminate with null
			++index; //skip last quote
			return retVal; //unqouted string
		}
		inline 
		bool isEndOfData(tJsonTypes token)
		{
			switch(token)
			{
				case eComma:
				case eCloseBracket:
				case eCloseCurly:
				case eInvalid:
					return true;
			}
			return false;
		}

		inline
		tJsonObjectPtr ParseNumber(const jsonString& json, int& index)
		{
			tJsonObjectPtr retVal((JsonObject*)NULL);
			int start = index;
			jsonChar buffer[255];
			int i=0;
			memset(buffer, 0, 255);
			bool decimal=false;
			bool exponent=false;
			bool signe = false;
			//NextToken does not increment the index if the value is alphanumeric
			for ( tJsonTypes token = NextToken(json, index); !isEndOfData(token); token = LookAhead(json, index))
			{
				if (decimal && json[index] == '.')
					return retVal; //only one decimal point allowed

				if (json[index] == '.')
					decimal = true;

				if (exponent && json[index] == 'E')
					return retVal; //only one exponent notation allowed

				if (json[index] == 'E')
				{
					exponent = true; 
					signe = false; //the exponent can be signed. reset flag in case it was set for the entire number
				}

				if (signe && (json[index] == '+'  || json[index] == '-'))
					return retVal; //signe cannot appear more then once

				if (json[index] == '+' || json[index] == '-')
					signe = true;

				buffer[i++] = json[index];
				++index;
			}

			if (decimal)
			{
				float data;
				retVal.reset(new JsonObject);
				sscanf(buffer, "%f", &data);
				retVal->_type = eFloat;
				retVal->_value = data;
			}
			else
			{
				int data;
				retVal.reset(new JsonObject);
				sscanf(buffer, "%d", &data);
				retVal->_type = eDecimal;
				retVal->_value = data;
			}
			return retVal;
		}

		template<typename CallbackT>
		tJsonObjectArrayPtr ParseArray(const jsonString& json, int& index, CallbackT& callback)
		{
			tJsonObjectArrayPtr jsonArray(new tJsonObjectArray);
			
			tJsonTypes token = NextToken(json, index);
			while (token != eCloseBracket && token != eInvalid)
			{
				tJsonObjectPtr obj = ParseValue(json, index, callback);
				if (obj != NULL)
					jsonArray->push_back(*obj);
				else
				{
					jsonArray.reset((tJsonObjectArray*)NULL);
					break;
				}
				token = NextToken(json, index);
			}
			return jsonArray;
		}

		template<typename CallbackT>
		tJsonObjectPtr ParseValue(const jsonString& json, int& index, CallbackT& callback)
		{
			tJsonObjectPtr retVal;
			retVal.reset((JsonObject*)NULL);
			if (!SkipWhiteSpace(json, index))
				return retVal;

			LOGD("message=-=", "ParseValue");
			tJsonTypes token = LookAhead(json, index);
			switch(token)
			{
				case eQuote:
				{
					tJsonStringPtr str = ParseString(json, index);
					if (str == NULL)
						break;
					retVal.reset(new JsonObject);
					retVal->_value = str;
					retVal->_type = eString;
					break;
				}
				case eOpenBracket:
					retVal.reset(new JsonObject);
					retVal->_value = ParseArray(json, index, callback);
					retVal->_type = eArray;
					break;

				case eDigit:
					retVal.reset(new JsonObject);
					retVal->_value = ParseNumber(json, index);
					retVal->_type = eNumber;
					break;

				case eOpenCurly:
					retVal.reset(new JsonObject);
					retVal->_value = ParseObject(json, index, callback);
					retVal->_type = eObject;
					break;

				case eBooleanTrue:
					retVal.reset(new JsonObject);
					retVal->_value = true;
					retVal->_type = eBool;
					break;

				case eBooleanFalse:
					retVal.reset(new JsonObject);
					retVal->_value = false;
					retVal->_type = eBool;
					break;
			}
			return retVal;
		}

		template<typename CallbackT>		
		tParsedJsonPtr ParseObject(const jsonString& json, int& index, CallbackT& callback)
		{
			tJsonObjectPtr value;
			tJsonTypes token;
			tParsedJsonPtr table(new tHashTable);

			tJsonStringPtr name;
			while (!_terminate && SkipWhiteSpace(json, index))
			{
				switch(NextToken(json, index))
				{
					case eInvalid: 
					case eCloseCurly:
						return table;

					case eComma:
						//object processing: after a comma the only two things allowed are a quote and an open curly
						token = LookAhead(json, index);
						if (token != eQuote && token != eOpenCurly)
							return table;

						if (token == eQuote)
						{
							name = ParseString(json, index);
							if (name == NULL) 
								return tParsedJsonPtr((tHashTable*)NULL);
						}
						break;

					case eOpenCurly:
					//case eQuote:
						name = ParseString(json, index);
						if (name == NULL) 
							return tParsedJsonPtr((tHashTable*)NULL);
						break;

					case eColon:
						// value
						value = ParseValue(json, index, callback);
						if (value != NULL)
						{
							(*table)[*name] = value;
							_terminate = callback(*name, *table);
						}
						break;
				}
			}
			return table;
		} 
	};
	inline
	bool ValidateJson(const JSON::jsonString& json)
	{
		int qouteCount=0, curlyCount=0, bracketCount=0;
		for (JSON::jsonString::const_iterator i=json.begin(); i!=json.end(); ++i)
		{
			switch(*i)
			{
				case '{':
					curlyCount++;
					break;
				case '}':
					curlyCount--;
					break;
				case '[':
					bracketCount++;
					break;
				case ']':
					bracketCount--;
					break;
			}
			if(curlyCount < 0 || bracketCount < 0)
				return false; //these should never be negetive
		}
		if (curlyCount != 0 || bracketCount != 0)
			return false;

		return true;
	}

	template<typename CallbackT>
	tParsedJsonPtr ParseJson(const jsonString& json, CallbackT& callback)
	{
		int index = 0;
		Parser parser;
		return parser.ParseObject(json, index, callback);
	}
}
#endif
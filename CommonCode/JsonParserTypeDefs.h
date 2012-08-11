#ifndef _JSON_PARSER_TYPE_DEFS_H_
#define _JSON_PARSER_TYPE_DEFS_H_
namespace JSON
{
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
		eAlphaNum,
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
	typedef TR1_NAMESPACE::shared_ptr<struct JsonObject> tJsonObjectPtr;
	typedef TR1_NAMESPACE::shared_ptr<tHashTable> tParsedJasonPtr;
	typedef std::vector<JsonObject> tJsonObjectArray;
	typedef TR1_NAMESPACE::shared_ptr<jsonString> tJsonStringPtr;

	struct JsonObject
	{
		tObjectType _type;
		boost::any _value;
	};

//****************** Forward Declarations **************************
	tJsonObjectPtr ParseArray(const jsonString& json, int& index);
	tJsonObjectPtr ParseNumber(const jsonString& json, int& index);
	tParsedJasonPtr ParseObject(const jsonString& json, int& index);
	tJsonObjectPtr ParseValue(const jsonString& json, int& index);
};
#endif
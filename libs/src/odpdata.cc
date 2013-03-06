#include <node.h>
#include <v8.h>
#include "odpdata.h"

using namespace v8;

OdpData::OdpData(){};
OdpData::~OdpData(){};

//Parameter Direction (System::Data::ParameterDirection)
	const int PARAM_IN 			= 1;
	const int PARAM_OUT 		= 2;
	const int PARAM_INOUT 		= 3;
	const int PARAM_RETURNVALUE = 6;

//OracleDb Types (Oracle::DataAccess::Client::OracleDbType)
	const int ORATYPE_BFILE 		= 101;
	const int ORATYPE_BLOB 			= 102;
	const int ORATYPE_BYTE 			= 103;
	const int ORATYPE_CHAR 			= 104;
	const int ORATYPE_CLOB 			= 105;
	const int ORATYPE_DATE 			= 106;
	const int ORATYPE_DECIMAL 		= 107;
	const int ORATYPE_DOUBLE 		= 108;
	const int ORATYPE_INT16 		= 111;
	const int ORATYPE_INT32 		= 112;
	const int ORATYPE_INT64       	= 113;
	const int ORATYPE_INTERVALDS 	= 114;
	const int ORATYPE_INTERVALYM 	= 115;
	const int ORATYPE_LONG 			= 109;
	const int ORATYPE_LONGRAW 		= 110;
	const int ORATYPE_NCHAR 		= 117;
	const int ORATYPE_NCLOB 		= 116;
	const int ORATYPE_NVARCHAR2 	= 119;
	const int ORATYPE_RAW 			= 120;
	const int ORATYPE_REFCURSOR 	= 121;
	const int ORATYPE_SINGLE 		= 122;
	const int ORATYPE_TIMESTAMP 	= 123;
	const int ORATYPE_TIMESTAMPLTZ 	= 124;
	const int ORATYPE_TIMESTAMPTZ 	= 125;
	const int ORATYPE_VARCHAR2 		= 126;
	const int ORATYPE_XMLTYPE 		= 127;

void OdpData::Init(Handle<v8::Object> target){
	//using a function just incase we need to add functionality later
	Local<v8::FunctionTemplate> funcTpl = v8::FunctionTemplate::New(New);
	funcTpl->SetClassName(v8::String::NewSymbol("OracleData"));
	funcTpl->InstanceTemplate()->SetInternalFieldCount(1);

	//template object for the parameters 
	Local<v8::ObjectTemplate> paramsTpl = v8::ObjectTemplate::New();
	
	paramsTpl->Set(v8::String::NewSymbol("INPUT"), v8::Number::New(PARAM_IN));
	paramsTpl->Set(v8::String::NewSymbol("OUTPUT"), v8::Number::New(PARAM_OUT));
	paramsTpl->Set(v8::String::NewSymbol("INPUTOUTPUT"), v8::Number::New(PARAM_INOUT));
	paramsTpl->Set(v8::String::NewSymbol("RETURNVALUE"), v8::Number::New(PARAM_RETURNVALUE));
	//template object for the types
	Local<v8::ObjectTemplate> typesTpl = v8::ObjectTemplate::New();

	typesTpl->Set(v8::String::NewSymbol("BFILE"), v8::Number::New(ORATYPE_BFILE));
	typesTpl->Set(v8::String::NewSymbol("BLOB"), v8::Number::New(ORATYPE_BLOB));
	typesTpl->Set(v8::String::NewSymbol("BYTE"), v8::Number::New(ORATYPE_BYTE));
	typesTpl->Set(v8::String::NewSymbol("CHAR"), v8::Number::New(ORATYPE_CHAR));
	typesTpl->Set(v8::String::NewSymbol("CLOB"), v8::Number::New(ORATYPE_CLOB));
	typesTpl->Set(v8::String::NewSymbol("DATE"), v8::Number::New(ORATYPE_DATE));
	typesTpl->Set(v8::String::NewSymbol("DECIMAL"), v8::Number::New(ORATYPE_DECIMAL));
	typesTpl->Set(v8::String::NewSymbol("DOUBLE"), v8::Number::New(ORATYPE_DOUBLE));
	typesTpl->Set(v8::String::NewSymbol("INT16"), v8::Number::New(ORATYPE_INT16));
	typesTpl->Set(v8::String::NewSymbol("INT32"), v8::Number::New(ORATYPE_INT32));
	typesTpl->Set(v8::String::NewSymbol("INT64"), v8::Number::New(ORATYPE_INT64));
	typesTpl->Set(v8::String::NewSymbol("INTERVALDS"), v8::Number::New(ORATYPE_INTERVALDS));
	typesTpl->Set(v8::String::NewSymbol("INTERVALYM"), v8::Number::New(ORATYPE_INTERVALYM));
	typesTpl->Set(v8::String::NewSymbol("LONG"), v8::Number::New(ORATYPE_LONG));
	typesTpl->Set(v8::String::NewSymbol("LONGRAW"), v8::Number::New(ORATYPE_LONGRAW));
	typesTpl->Set(v8::String::NewSymbol("RAW"), v8::Number::New(ORATYPE_RAW));
	typesTpl->Set(v8::String::NewSymbol("REFCURSOR"), v8::Number::New(ORATYPE_REFCURSOR));
	typesTpl->Set(v8::String::NewSymbol("NVARCHAR2"), v8::Number::New(ORATYPE_NVARCHAR2));
	typesTpl->Set(v8::String::NewSymbol("SINGLE"), v8::Number::New(ORATYPE_SINGLE));
	typesTpl->Set(v8::String::NewSymbol("TIMESTAMP"), v8::Number::New(ORATYPE_TIMESTAMP));
	typesTpl->Set(v8::String::NewSymbol("TIMESTAMPLTZ"), v8::Number::New(ORATYPE_TIMESTAMPLTZ));
	typesTpl->Set(v8::String::NewSymbol("TIMESTAMPTZ"), v8::Number::New(ORATYPE_TIMESTAMPTZ));
	typesTpl->Set(v8::String::NewSymbol("VARCHAR2"), v8::Number::New(ORATYPE_VARCHAR2));
	typesTpl->Set(v8::String::NewSymbol("XMLTYPE"), v8::Number::New(ORATYPE_XMLTYPE));


	funcTpl->Set(v8::String::NewSymbol("parameterDirection"), paramsTpl->NewInstance());
	funcTpl->Set(v8::String::NewSymbol("dbType"), typesTpl->NewInstance());

	Persistent<v8::Function> constructor = Persistent<v8::Function>::New(funcTpl->GetFunction());
	target->Set(v8::String::NewSymbol("OracleData"), constructor);
}

v8::Handle<v8::Value> OdpData::New(const Arguments& args){
	return args.This();
}
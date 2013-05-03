#include <node.h>
#include <v8.h>
#include <iostream>
#include <string>
#include <sstream>
#include "odpcommand.h"
#include "odpconnection.h"
#include "helpers/helpers.h"

#using <System.dll>
#using <System.Data.dll>
#using <Oracle.DataAccess.dll>

using namespace std;
using namespace v8;
using namespace Helpers;
using namespace System;
using namespace System::Data;
using namespace Oracle::DataAccess::Client;
using namespace Oracle::DataAccess::Types;

OdpCommand::OdpCommand(){}; //constructor
OdpCommand::~OdpCommand(){}; //destructor
	//Command Types (System::Data::CommandType)
	const int CMDTYPETEXT 	= 1;
	const int CMDTYPESTDPROC 	= 4;

void OdpCommand::Init(Handle<v8::Object> target){
	Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
	tpl->SetClassName(v8::String::NewSymbol("OracleCommand"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);
	//attach constant properties the template object
	Local<v8::ObjectTemplate> otpl = v8::ObjectTemplate::New();
	
	otpl->Set(v8::String::NewSymbol("TEXT"), v8::Number::New(CMDTYPETEXT));
	otpl->Set(v8::String::NewSymbol("STOREDPROCEDURE"), v8::Number::New(CMDTYPESTDPROC));

	tpl->Set(v8::String::NewSymbol("commandType"), otpl->NewInstance());
	//set attach functions to the prototype 
	tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("executeReader"), FunctionTemplate::New(ExecuteReader)->GetFunction());
	tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("executeNonQuery"), FunctionTemplate::New(ExecuteNonQuery)->GetFunction());
	tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("executeScalar"), FunctionTemplate::New(ExecuteScalar)->GetFunction());

	tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("Dispose"), FunctionTemplate::New(Dispose)->GetFunction());

	Persistent<Function> constructor = Persistent<Function>::New(tpl->GetFunction());
	target->Set(v8::String::NewSymbol("OracleCommand"), constructor);
}

Handle<Value> OdpCommand::New(const Arguments& args){
	HandleScope scope;
	OdpCommand* c = new OdpCommand();
	Oracle::DataAccess::Client::OracleCommand^ cmd;

	//To handle parameters
	Handle<v8::Object> v8Con;
	Handle<v8::Array> params = v8::Array::New(0);
	int cmdType = 0;
	System::String^ cmdText;

	//Validation
	switch(args.Length()){
		case 4:
			if(!args[0]->IsString() || !args[1]->IsNumber() || !args[2]->IsArray() || !args[3]->IsObject()){
				ThrowException(v8::Exception::TypeError(v8::String::New("Invalid arguments")));
				return scope.Close(Undefined());
			}
			cmdType = args[1]->Uint32Value();
			params = Handle<v8::Array>::Cast(args[2]);
			v8Con = args[3]->ToObject();
			break;
		case 3:
			if(!args[0]->IsString() || (!args[1]->IsNumber() && !args[1]->IsArray()) || !args[2]->IsObject()){
				ThrowException(v8::Exception::TypeError(v8::String::New("Invalid arguments")));
				return scope.Close(Undefined());
			}
			if(args[1]->IsNumber()){
				cmdType = args[1]->Uint32Value();
			}else{
				params = Handle<v8::Array>::Cast(args[1]);
			}
			v8Con = args[2]->ToObject();
			break;
		case 2:
			if(!args[0]->IsString() || !args[1]->IsObject()){
				ThrowException(v8::Exception::TypeError(v8::String::New("Invalid arguments")));
				return scope.Close(Undefined());
			}
			v8Con = args[1]->ToObject();
			break;
		default:
			ThrowException(v8::Exception::TypeError(v8::String::New("Incomplete number of arguments")));
			return scope.Close(Undefined());
			break;
	}
	cmdText = Helpers::String::ToSystemString(args[0]->ToString());

	OdpConnection* ccn = node::ObjectWrap::Unwrap<OdpConnection>(v8Con);

	cmd = gcnew Oracle::DataAccess::Client::OracleCommand();
	cmd->Connection = ccn->connection;
	cmd->CommandText = cmdText;

	switch(cmdType){
		case CMDTYPETEXT:
			cmd->CommandType = CommandType::Text;
			break;
		case CMDTYPESTDPROC:
			cmd->CommandType = CommandType::StoredProcedure;
			break;
		case 0:
			cmd->CommandType = CommandType::Text;
			break; 
		default:
			ThrowException(v8::Exception::TypeError(v8::String::New("Invalid CommandType")));
			return scope.Close(Undefined());
			break;
	}
	
	//Add parameters if they exist
	AddParameters(params, cmd);

	c->command = cmd;

	c->Wrap(args.This());
	return args.This();
}

void OdpCommand::AddParameters(Handle<v8::Array> parameters, Oracle::DataAccess::Client::OracleCommand^ &command){
	HandleScope scope;
	Handle<v8::Object> p;
	System::String^ strName;
	System::Object^ objValue;

	if(parameters->Length() > 0){	
		//define the parameters on the collection
		Handle<v8::Value> name;
		Handle<v8::Value> type;
		Handle<v8::Value> size;
		Handle<v8::Value> value;
		Handle<v8::Value> direction;

		for(unsigned int i = 0; i < parameters->Length(); i++){
			p = parameters->Get(i)->ToObject(); 
			//implement good type checking here
			name = p->Get(v8::String::New("name"));
			type = p->Get(v8::String::New("type"));
			size = p->Get(v8::String::New("size"));
			value = p->Get(v8::String::New("value"));
			direction = p->Get(v8::String::New("direction"));

			if(name->IsUndefined()){
				ThrowException(v8::Exception::TypeError(v8::String::New("No name property defined.")));
			}
			if(type->IsUndefined()){
				ThrowException(v8::Exception::TypeError(v8::String::New("No type property defined.")));
			}
			if(direction->IsUndefined()){
				ThrowException(v8::Exception::TypeError(v8::String::New("No direction property defined.")));
			}

			strName = Helpers::String::ToSystemString(name->ToString());
			if(value->IsString()){
				objValue = Helpers::String::ToSystemString(value->ToString());
			}else if(value->IsNumber()){
				objValue = static_cast<System::Int32>(value->Uint32Value());
			}else {}

			try{
				if(size->IsUndefined() && value->IsUndefined()){
					command->Parameters->Add(strName, static_cast<OracleDbType>(type->Uint32Value()), static_cast<ParameterDirection>(direction->Uint32Value()));
				}else if (!size->IsUndefined() && value->IsUndefined()){
					command->Parameters->Add(strName, static_cast<OracleDbType>(type->Uint32Value()), static_cast<System::Int32>(size->Uint32Value()), static_cast<ParameterDirection>(direction->Uint32Value()));
				}else if(!size->IsUndefined() && !value->IsUndefined()){
					command->Parameters->Add(strName, static_cast<OracleDbType>(type->Uint32Value()), static_cast<System::Int32>(size->Uint32Value()), objValue, static_cast<ParameterDirection>(direction->Uint32Value()));
				}else if(size->IsUndefined() && !value->IsUndefined()){
					command->Parameters->Add(strName, static_cast<OracleDbType>(type->Uint32Value()), objValue, static_cast<ParameterDirection>(direction->Uint32Value()));
				}else{}
			}catch(ArgumentException^ e){
				ThrowException(v8::Exception::TypeError(Helpers::String::ToV8String(e->Message)));
			}
		}
	}
}

Handle<v8::Value> OdpCommand::ToJSON(Oracle::DataAccess::Client::OracleDataReader^ reader){
	HandleScope scope;
	Handle<v8::String> rowset = v8::String::New("[");
	Handle<v8::String> rows;
	int fc = 0; //fields count
	int rc = 0; //records count
	System::String^ items;
	//loop through the resultset , used for RefCursors	
	do{
		rows = v8::String::Concat(rows, v8::String::New("["));
		//loop through the rows
		while(reader->Read()){
			//loop through the columns
			rows = v8::String::Concat(rows, v8::String::New("{"));
			while(fc < reader->FieldCount){
				items = "\"" + Helpers::String::Replace(reader->GetName(fc),".","_") + "\":\"" + reader[fc] + "\"";
				if(fc != reader->FieldCount - 1){
					items = items + ",";
				}
				rows = v8::String::Concat(rows, Helpers::String::ToV8String(items));
				fc++;
			}
			fc = 0;
			rows = v8::String::Concat(rows, v8::String::New("},"));
			rc++;	
		}
		//remove the comma fronm the last JSON item
		rows = Helpers::String::SnipEnd(rows); 
		rows = v8::String::Concat(rows, v8::String::New("],"));
	}while (reader->NextResult())
	rows = Helpers::String::SnipEnd(rows);
	rowset = v8::String::Concat(rowset, rows);
	rowset = v8::String::Concat(rowset, v8::String:New("]"));
	return scope.Close(Helpers::Json::ParseJson(rowset));
}

//Builds and returns a javascript object rather than parsing to json
Handle<v8::Value> OdpCommand::ToJSON(Oracle::DataAccess::Client::OracleParameterCollection^ parameters){
	HandleScope scope;
	Oracle::DataAccess::Client::OracleParameter^ parameter;
	Handle<v8::Object> params = v8::Object::New();
	System::String^ param;
	//unwrap to access the object
	OracleClob^ pClob;

	if(parameters->Count > 0){
		for(int i = 0; i < parameters->Count; i++){	
			parameter = parameters->default[i];
			if(parameter->OracleDbType == OracleDbType::Clob){
				pClob = dynamic_cast<OracleClob^>(parameter->Value);
				params->Set(Helpers::String::ToV8String(parameter->ParameterName), Helpers::Json::ParseJson(Helpers::String::ToV8String(pClob->Value)));		
			}else{
				param = parameter->Value + "";
				params->Set(Helpers::String::ToV8String(parameter->ParameterName), Helpers::String::ToV8String(param));
			}			 		
		}	
	}
	return scope.Close(params);
}

Handle<Value> OdpCommand::ExecuteReader(const Arguments& args){
	HandleScope scope;
	Oracle::DataAccess::Client::OracleDataReader^ reader;
	Local<Function> callback = Local<Function>::Cast(args[0]);
	Handle<v8::Value> error = Undefined();
	Handle<v8::Value> rows;
	const unsigned argc = 2;
	//unwrap to access the object
	OdpCommand* c = ObjectWrap::Unwrap<OdpCommand>(args.This());
	try{
		//c->command->Connection->Open();
		reader = c->command->ExecuteReader();
		rows = ToJSON(reader);		
	}catch(OracleException ^e){
		error = Helpers::String::ToV8String(e->Message);
		rows = Undefined();
	}

	//set up the variables for the callback function
	Local<Value> argv[argc] = { Local<v8::Value>::New(error), Local<v8::Value>::New(rows) };
	callback->Call(Context::GetCurrent()->Global(), argc, argv);
	
	reader->Close();
	//c->command->Connection->Close();
	return scope.Close(Undefined());
}

Handle<Value> OdpCommand::ExecuteNonQuery(const Arguments& args){
	HandleScope scope;
	Local<Function> callback = Local<Function>::Cast(args[0]);
	Handle<v8::Value> error = Undefined();
	Handle<v8::Value> params = Undefined();
	const unsigned argc = 3;
	int rowsAffected = -2;
	//unwrap to access the object
	OdpCommand* c = ObjectWrap::Unwrap<OdpCommand>(args.This());
	try{
		//c->command->Connection->Open();	
 		rowsAffected = c->command->ExecuteNonQuery();
		params = ToJSON(c->command->Parameters);
	}catch(OracleException^ e){
		error = Helpers::String::ToV8String(e->Message);
		params = Undefined();
	}
	Local<Value> argv[argc] = {Local<Value>::New(error), Local<v8::Value>::New(Helpers::String::ToV8String(rowsAffected)), Local<v8::Value>::New(params)};
	callback->Call(Context::GetCurrent()->Global(), argc, argv);
	//c->command->Connection->Close();
	return scope.Close(Undefined());
}

Handle<Value> OdpCommand::ExecuteScalar(const Arguments& args){
	HandleScope scope;
	Local<Function> callback = Local<Function>::Cast(args[0]);
	Handle<v8::Value> error = Undefined();
	const unsigned argc = 2;

	System::String^ rows;
	//unwrap to access the object
	OdpCommand* c = ObjectWrap::Unwrap<OdpCommand>(args.This());
	try{
		//c->command->Connection->Open();
		rows = (System::String^) c->command->ExecuteScalar();		
	}catch(OracleException ^e){
		error = Helpers::String::ToV8String(e->Message);
	} 
	
	Local<Value> argv[argc] = { Local<Value>::New(error), Local<Value>::New(Helpers::String::ToV8String(rows)) };
	callback->Call(Context::GetCurrent()->Global(), argc, argv);
	//Close the connection
	//c->command->Connection->Close();
	//destroy the handle scope and all the handles within the scope
	return scope.Close(Undefined());
}



Handle<Value> OdpCommand::Dispose(const Arguments& args){
	HandleScope scope;
	//OdpCommand* c = node::ObjectWrap::Unwrap<OdpCommand>(args.This());
	//c->command->Dispose();
	return scope.Close(v8::String::New("Command Disposed."));
}

#include <node.h>
#include <v8.h>
#include <iostream>
#include <string>

#include "odpconnection.h"
#include "helpers/helpers.h"

//dynamically linked libraries
#using <System.dll>
#using <System.Data.dll>
#using <Oracle.DataAccess.dll>

using namespace std;
using namespace v8;
using namespace Helpers;
using namespace System;
using namespace System::Data;
using namespace Oracle::DataAccess::Client;

OdpConnection::OdpConnection(){}; //constructor
OdpConnection::~OdpConnection(){}; //destructor

void OdpConnection::Init(Handle<v8::Object> target){
	Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
	tpl->SetClassName(v8::String::NewSymbol("OracleConnection"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1); //only one instance of the Oracle Connections is allowed?

	//set attach functions to the prototype 
	tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("open"), FunctionTemplate::New(Open)->GetFunction());
	tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("close"), FunctionTemplate::New(Close)->GetFunction());
	tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("dispose"), FunctionTemplate::New(Dispose)->GetFunction());
	//set the constructor

	Persistent<Function> constructor = Persistent<Function>::New(tpl->GetFunction());
	target->Set(v8::String::NewSymbol("OracleConnection"), constructor);

}
//methods of the odpconnection class
Handle<Value> OdpConnection::New(const Arguments& args){
	HandleScope scope;
	OdpConnection* o = new OdpConnection();
	Oracle::DataAccess::Client::OracleConnection^ cn;
	if(args[0]->IsString()){
		try {
			cn = gcnew Oracle::DataAccess::Client::OracleConnection(Helpers::String::ToSystemString(args[0]->ToString()));
		} catch (System::Exception ^e){
			ThrowException(v8::Exception::Error(Helpers::String::ToV8String(e->Message)));
			return scope.Close(Undefined());
		}
	}else{
		ThrowException(v8::Exception::TypeError(v8::String::New("String type arguments expected")));
		return scope.Close(Undefined());
	}
	o->connection = cn;
	//wrap to make the object avaliable to other methods
	o->Wrap(args.This());
	return args.This();
}

Handle<Value> OdpConnection::Open(const Arguments& args){
	HandleScope scope;
	//unwrap to access the object
	OdpConnection* o = ObjectWrap::Unwrap<OdpConnection>(args.This());
	try{
		o->connection->Open();
		cout << "Connection Open" << endl;
	}catch(OracleException ^e){
		ThrowException(v8::Exception::Error(Helpers::String::ToV8String(e->Message)));
	}
	return scope.Close(Undefined());
}

Handle<Value> OdpConnection::Close(const Arguments& args){
	HandleScope scope;
	//unwrap to access the object
	OdpConnection* o = ObjectWrap::Unwrap<OdpConnection>(args.This());
	try{
		o->connection->Close();
		cout << "Connection Closed" << endl;
	}catch(System::Exception ^e){
		ThrowException(v8::Exception::Error(Helpers::String::ToV8String(e->Message)));
	}
	return scope.Close(Undefined());
}

Handle<Value> OdpConnection::Dispose(const Arguments& args){
	HandleScope scope;
	//unwrap the object for access
	OdpConnection* o = ObjectWrap::Unwrap<OdpConnection>(args.This());
	o->connection->~OracleConnection();
	cout << "Connection Disposed" << endl;
	return scope.Close(Undefined());
}
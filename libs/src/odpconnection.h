#ifndef ODPCONNECTION_H
#define ODPCONNECTION_H

#include <node.h>
#include <v8.h>
#include <vcclr.h>

#using <Oracle.DataAccess.dll>

class OdpConnection : public node::ObjectWrap {
	public:
		static void Init(v8::Handle<v8::Object> target);
		gcroot<Oracle::DataAccess::Client::OracleConnection^> connection;
		static v8::Handle<v8::Value> Open(const v8::Arguments& args);
		static v8::Handle<v8::Value> Close(const v8::Arguments& args);
		static v8::Handle<v8::Value> Dispose(const v8::Arguments& args);
	private:
		OdpConnection(); //constructor
		~OdpConnection(); //destructor

		static v8::Handle<v8::Value> New(const v8::Arguments& args);		
};

#endif
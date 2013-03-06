#ifndef ODPCOMMAND_H
#define ODPCOMMAND_H

#include <node.h>
#include <v8.h>
#include <vcclr.h>

#using <Oracle.DataAccess.dll>

class OdpCommand : public node::ObjectWrap {
	public:
		static void Init(v8::Handle<v8::Object> target);
	private:
		OdpCommand();
		~OdpCommand();
		//methods
		static v8::Handle<v8::Value> New(const v8::Arguments& args);
		static v8::Handle<v8::Value> Dispose(const v8::Arguments& args);
		static v8::Handle<v8::Value> ExecuteNonQuery(const v8::Arguments& args);
		static v8::Handle<v8::Value> ExecuteReader(const v8::Arguments& args);
		static v8::Handle<v8::Value> ExecuteScalar(const v8::Arguments& args);

		static v8::Handle<v8::Value> ToJSON(Oracle::DataAccess::Client::OracleDataReader^ reader);
		static v8::Handle<v8::Value> ToJSON(Oracle::DataAccess::Client::OracleParameterCollection^ parameters);

		static void AddParameters(v8::Handle<v8::Array> parameters, Oracle::DataAccess::Client::OracleCommand^ &command);
		gcroot<Oracle::DataAccess::Client::OracleCommand^> command;
};
#endif
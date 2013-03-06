#ifndef ODPDATA_H
#define ODPDATA_H

#include <node.h>
#include <v8.h>

class OdpData : public node::ObjectWrap{
	public:
		static void Init(v8::Handle<v8::Object> target);
	private:
		OdpData();
		~OdpData();
		static v8::Handle<v8::Value> New(const v8::Arguments& args);
};

#endif
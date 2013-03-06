#include <v8.h>
#include <iostream>
#include <string>
#include <sstream>
#include "helpers.h"

#using <System.dll>

using namespace std;
using namespace v8;
using namespace System;

Helpers::String::String(){};
Helpers::String::~String(){};

void Helpers::String::MarshalString ( System::String ^ s, string& os ) {
   using namespace Runtime::InteropServices;
   const char* chars = 
      (const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
   os = chars;
   Marshal::FreeHGlobal(IntPtr((void*)chars));
}
void Helpers::String::MarshalString ( System::String ^ s, wstring& os ) {
   using namespace Runtime::InteropServices;
   const wchar_t* chars = 
      (const wchar_t*)(Marshal::StringToHGlobalUni(s)).ToPointer();
   os = chars;
   Marshal::FreeHGlobal(IntPtr((void*)chars));
}
//convert from v8::String to std::string
std::string Helpers::String::ToStdString(Handle<v8::String> s){
	return std::string(*v8::String::Utf8Value(s));
}
std::string Helpers::String::ToStdString(System::String^ s){
	std::string str;
	MarshalString(s, str);
	return str;
}

//takes in an integer and spits out a string
std::string Helpers::String::ToStdString(int i){
	std::stringstream out;
	out << i;
	return out.str();
}
//convert from std::string to System::String
System::String^ Helpers::String::ToSystemString(std::string s){
	System::String^ cs = gcnew System::String(s.c_str());
	return cs;
}
//convert from v8::string to System::String
System::String^ Helpers::String::ToSystemString(Handle<v8::String> s){
	std::string cs = ToStdString(s);
	System::String^ ss = ToSystemString(cs);
	return ss;
}
//convert to v8::string handle
Handle<v8::String> Helpers::String::ToV8String(std::string& s){
	return v8::String::New(s.c_str());
}

Handle<v8::String> Helpers::String::ToV8String(System::String ^ s){
	std::string cs = "";
	MarshalString(s, cs);
	return ToV8String(cs);
}

Handle<v8::String> Helpers::String::ToV8String(int i){
	std::string iout = ToStdString(i);
	return ToV8String(iout);
}

std::string Helpers::String::Replace(std::string main,std::string find , std::string replace){
	unsigned found;
	found = main.find(find);
	while(found < main.length()){
		main.replace(found, 1, replace);
		found = main.find(find, found + 1);
	}
	return main;
}
System::String^ Helpers::String::Replace(System::String^ main,std::string find , std::string replace){
	std::string m = ToStdString(main);
	return ToSystemString(Replace(m, find, replace));
}
Handle<v8::String> Helpers::String::Replace(Handle<v8::String> main,std::string find , std::string replace){
	std::string m = ToStdString(main);
	return ToV8String(Replace(m, find, replace));
}

Handle<v8::String> Helpers::String::SnipEnd(Handle<v8::String> s){
	std::string cs = ToStdString(s);
	char* c = (char*)cs.c_str();
	c[strlen(c)-1] = 0;
	std::string os(c);
	return ToV8String(os);
}

Handle<v8::String> Helpers::String::SnipStart(Handle<v8::String> s){
	std::string cs = ToStdString(s);
	char* c = (char*)cs.c_str(); //converts to a const character pointer then typecast to cha r pointer
	c++;
	std::string os(c);
	return ToV8String(os);
}

Local<v8::Value> Helpers::Json::ParseJson(Handle<v8::Value> jsonString){
	HandleScope scope;

	Handle<Context> context = Context::GetCurrent();
	Handle<v8::Object> global = context->Global();
	Local<v8::Value> json;
	try{
		Handle<v8::Object> JSON = global->Get(v8::String::New("JSON"))->ToObject();
		Handle<v8::Function> JSON_parse = Handle<v8::Function>::Cast(JSON->Get(v8::String::New("parse")));
		json = JSON_parse->Call(JSON, 1, &jsonString);
	}catch(v8::Exception e){
		ThrowException(v8::Exception::Error(v8::String::New("Unable to create Json object, invalid data content.")));
		return scope.Close(Undefined());
	}
	return scope.Close(json);
}
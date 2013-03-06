#ifndef HELPERS_H
#define HELPERS_H
#include <v8.h>
#include <iostream>
#include <string>
#using <System.dll>

using namespace std;
using namespace v8;
using namespace System;

namespace Helpers 
{
	class String {
		public:
			static void MarshalString ( System::String ^ s, string& os );
			static void MarshalString ( System::String ^ s, wstring& os );

			static std::string ToStdString(Handle<v8::String> s);
			static std::string ToStdString(System::String^ s);
			static std::string ToStdString(int i);

			static System::String^ ToSystemString(std::string s);
			static System::String^ ToSystemString(Handle<v8::String> s);

			static Handle<v8::String> ToV8String(std::string& s);
			static Handle<v8::String> ToV8String(System::String ^ s);
			static Handle<v8::String> ToV8String(int i);

			static Handle<v8::String> SnipEnd(Handle<v8::String> s);
			static Handle<v8::String> SnipStart(Handle<v8::String> s);

			static std::string Replace(std::string main, std::string find, std::string replace);
			static System::String^ Replace(System::String^ main, std::string find, std::string replace);
			static Handle<v8::String> Replace(Handle<v8::String> main, std::string find, std::string replace);
		private:
			String();
			~String();
	};
	class Json {
		public:
			static Local<v8::Value> ParseJson(Handle<Value> jsonString);
		private:
			Json();
	};
};
#endif
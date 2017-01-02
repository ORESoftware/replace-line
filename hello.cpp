
#include <nan.h>
#include <node.h>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

//using namespace v8;
using namespace std;

void Method(const v8::FunctionCallbackInfo<v8::Value>& args) {


//  if (info.Length() < 2) {
//    Nan::ThrowTypeError("Wrong number of arguments");
//    return;
//  }
//
//  if (!info[0]->IsNumber() || !info[1]->IsNumber()) {
//    Nan::ThrowTypeError("Wrong arguments");
//    return;
//  }
//
//  double arg0 = info[0]->NumberValue();
//  double arg1 = info[1]->NumberValue();
//  v8::Local<v8::Number> num = Nan::New(arg0 + arg1);


fstream f;
//infile.open("/home/oleg/dogs.txt", fstream::in | fstream::out);
f.open("/home/oleg/dogs.txt", fstream::in);

if(f.fail()){
  cerr << " infile fail" << endl;
  exit(1);
}

if(!f.is_open()){
  cerr << " file could not be opened " << endl;
  exit(1);
}

vector<string> v;
vector<vector<int>> ints;

string line;

while (std::getline(f, line)){


   string str(line);
   if(true){

       long position = f.tellp();
       cout << "tellp position is " << position << endl;

       v.push_back(str);
        int len = str.length() + 1;

       vector<int> c = {position - len,len};
       ints.push_back(c);

       cout << " => line contains => " << line << endl;
       cout << " line length is " << len << endl;


   }

}

f.close();

fstream z;
z.open("/home/oleg/dogs.txt");


  v8::Isolate* isolate = args.GetIsolate();
//    v8::Local<String> retval = String::NewFromUtf8(isolate, "world");
    v8::Local<v8::Array> a = v8::Array::New(isolate);




for(std::vector<int>::size_type i = 0; i < v.size(); i++) {


//    v8::Local<String> vvv = v[i];

//    v8::Local<v8::String> hTextJS = v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), v[i].c_str());

//    a->Set(i, v8::String::NewFromUtf8(isolate, hTextJS));
//    std::string vvv = v[i];


//    v8::Local<v8::String> prop = v8::String(isolate, "dog");

    a->Set(i, v8::String::NewFromUtf8(isolate, v[i].c_str()));

    int pos = ints[i][0];
    int len = ints[i][1];

     z.seekp (pos);
     string s(len, ' ');
      z << s;

}


  z.close();

    args.GetReturnValue().Set(a);

}

void init(v8::Local<v8::Object> exports) {
NODE_SET_METHOD(exports, "hello", Method);
}

NODE_MODULE(hello_addon, init)
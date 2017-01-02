
#include <nan.h>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

using namespace std;


void Add(const Nan::FunctionCallbackInfo<v8::Value>& info) {

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


for(std::vector<int>::size_type i = 0; i < v.size(); i++) {

    int pos = ints[i][0];
    int len = ints[i][1];

     z.seekp (pos);
     string s(len, ' ');
      z << s;

}


  z.close();

//    std::string* ret(v.begin(), v.end());

  v8::Local<v8::Object> obj = Nan::New<v8::Object>();
//  obj->Set(Nan::New("zzzzzz").ToLocalChecked(), info[0]->ToString());
//    v8::Local<Value> argv[] = { Null(), v };
    obj->Set(Nan::New("zzzzzz").ToLocalChecked(), info[0]->ToString());
    info.GetReturnValue().Set(obj);

}


void Init(v8::Local<v8::Object> exports, v8::Local<v8::Object> module) {


  exports->Set(Nan::New("hello").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(Add)->GetFunction());


// module->Set(Nan::New("hello").ToLocalChecked(),
//      Nan::New<v8::FunctionTemplate>(Method)->GetFunction());

}

NODE_MODULE(hello, Init)
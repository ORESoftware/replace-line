
#include <nan.h>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

using namespace std;

void Method(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  info.GetReturnValue().Set(Nan::New("world").ToLocalChecked());
}


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
// innert array is position, length
vector<vector<int>> ints;

string line;

int count = 0;
while (std::getline(f, line)){

   count++;
   cout << line << endl;

   string str(line);
   if(true){


       long position = f.tellp();
       cout << "tellp position is " << position << endl;

       v.push_back(str);

       // add one to represent new line char
//       int len = str.length() + 1;
        int len = str.length() + 1;

       vector<int> c = {position - len,len};
       ints.push_back(c);

//       ints.insert(ints.end(),position,len);

       cout << " => line contains => " << line << endl;
       cout << " line length is " << len << endl;


   }

}


 cout << " => count => " << count << endl;
f.close();

//ofstream z;
//z.open("/home/oleg/dogs.txt", ofstream::in);
fstream z;
z.open("/home/oleg/dogs.txt");


for(std::vector<int>::size_type i = 0; i < v.size(); i++) {


//    cout << " love " << v[i] << endl;
//    cout << " love " << ints[i][0] << " DDDDD " << ints[i][1] << endl;

    int pos = ints[i][0];
    int len = ints[i][1];

     z.seekp (pos);
     string s(len, ' ');
//     z.put(s);

      z << s;

}

 cout << " => count 2 => " << count << endl;

z.close();

  v8::Local<v8::Object> obj = Nan::New<v8::Object>();
  obj->Set(Nan::New(v).ToLocalChecked(), info[0]->ToString());

  info.GetReturnValue().Set(obj);
//  info.GetReturnValue().Set('d');
}


void Init(v8::Local<v8::Object> exports, v8::Local<v8::Object> module) {


  exports->Set(Nan::New("hello").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(Add)->GetFunction());

// exports->Set(Nan::New("add").ToLocalChecked(),
//               Nan::New<v8::FunctionTemplate>(Add)->GetFunction());

// module->Set(Nan::New("hello").ToLocalChecked(),
//      Nan::New<v8::FunctionTemplate>(Method)->GetFunction());

}

NODE_MODULE(hello, Init)
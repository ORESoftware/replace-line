
#include <nan.h>
#include <node.h>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <regex>
#include <regex.h>

//using namespace v8;
using namespace std;

//logfile = sys.argv[1]
//regex = json.loads(sys.argv[2]);
//max = int(sys.argv[3])
//priority = int(sys.argv[4])
//priority_max = int(sys.argv[5])


void Method(const v8::FunctionCallbackInfo<v8::Value>& args) {

  if (args.Length() < 2) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  }

fstream f;

  if (!args[0]->IsString()) {
    Nan::ThrowTypeError("First argument to 'replace-line' must be the absolute file path.");
    return;
  }

  //infile.open("/home/oleg/dogs.txt", fstream::in | fstream::out);
//  f.open("/home/oleg/dogs.txt", fstream::in);
    v8::String::Utf8Value filepath(args[0]->ToString());
    f.open(*filepath, fstream::in);



//std::regex reg1;

  if (!args[1]->IsString()) {
    Nan::ThrowTypeError("Second argument to 'replace-line' must be a string regular expression.");
    return;
  }


    v8::String::Utf8Value target(args[1]);
    std::string trgt(*target);
    std::regex reg1(trgt);

//    int count = args[2]->ToNumber()->NumberValue();
//    int priority = args[3]->ToNumber()->NumberValue();
//    int priority_search_cap = args[4]->ToNumber()->NumberValue();

    int count = 1;
    int priority = args[3]->ToNumber()->NumberValue();
    int priority_search_cap = args[4]->ToNumber()->NumberValue();

    cout << " count => " << count << endl;
    cout << " priority => " << priority << endl;
    cout << " priority_search_cap => " << priority_search_cap << endl;


//  double arg0 = info[0]->NumberValue();
//  double arg1 = info[1]->NumberValue();
//  v8::Local<v8::Number> num = Nan::New(arg0 + arg1);





if(f.fail()){
  cerr << " => 'replace-line' usage error =>  infile fail => path to file was likely invalid. " << endl;
  exit(1);
}

if(!f.is_open()){
  cerr << " file could not be opened " << endl;
  exit(1);
}

vector<string> v;
vector<vector<int>> ints;

string line;
int index = 0;

while (getline(f, line) && index < count){

     string str(line);
     std::smatch match;

     if (std::regex_search(str, match, reg1) && match.size() > 0) {

        index++;

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
z.open(*filepath);


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
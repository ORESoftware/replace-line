
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
//    int priority = args[3]->ToNumber()->NumberValue();
//    int priority_search_cap = args[4]->ToNumber()->NumberValue();
//
//    cout << " count => " << count << endl;
//    cout << " priority => " << priority << endl;
//    cout << " priority_search_cap => " << priority_search_cap << endl;


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

     long old_offset = f.tellp()-str.length() -1;

     if (std::regex_search(str, match, reg1) && match.size() > 0) {

        index++;
       long new_offset = f.tellp();

       v.push_back(str);

        if(old_offset > 1){
           old_offset--;
         }

        long len = new_offset -old_offset-1;
       vector<int> c = {old_offset,len};
       ints.push_back(c);
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
//     s = "\n" + s;
      z << s;

}


  z.close();


  fstream x;
  x.open(*filepath, fstream::in);

  int ind = 0;

  int off = -1;
  int leng = -1;

  while (getline(x, line) && ind < 1){

     string str(line);

     char h = str[0];
     char p = str[str.length()-1];

//       cout << " p " << p << endl;

       bool hIsChar = std::isspace(static_cast<unsigned char>(h));
       bool pIsChar = std::isspace(static_cast<unsigned char>(p));

    if (hIsChar && !pIsChar) {
//        cout << " c is a space " << endl;
        leng = str.length();
//         cout << " leng " << leng << endl;

        off = x.tellp()- leng;

//        cout << " off " << off << endl;
    }

  }

  x.close();


  if(off > -1){

//  cout << " off is on " << endl;

fstream m;
m.open(*filepath);

   m.seekp (off);
     string xx(leng + 1, ' ');
//     s = "\n" + s;
      m << xx;

    m.close();
  }



    args.GetReturnValue().Set(a);

}

void init(v8::Local<v8::Object> exports) {
NODE_SET_METHOD(exports, "hello", Method);
}

NODE_MODULE(hello_addon, init)
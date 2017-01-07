
#include <nan.h>
#include <node.h>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <regex>
#include <regex.h>
#include "json.hpp"

// for convenience
using json = nlohmann::json;

//using namespace v8;
using namespace std;

//logfile = sys.argv[1]
//regex = json.loads(sys.argv[2]);
//max = int(sys.argv[3])
//priority = int(sys.argv[4])
//priority_max = int(sys.argv[5])

string trim(const string& str)
{
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

struct CustomSort{

    bool operator()(tuple<int,regex> const &lhs, tuple<int,regex> const &rhs){
        return std::get<0>(lhs) > std::get<0>(rhs);
    }
};


vector<regex> FindPriorityItems(int priority, vector<string> v, int count, int numberOfRecursiveCalls){

       std::vector<regex> ret;
       std::vector<std::tuple<int, regex>> ints;

        for(std::vector<int>::size_type i = 0; i < v.size(); i++) {
             auto j = json::parse(trim(v[i]));
             int pt = j["priority"];
             string uidStr = j["uid"];
             std::regex reg1(uidStr);
             ints.push_back(std::make_tuple(pt,reg1));
        }

        // sort ints
        int size = ints.size();
        //std::sort(begin(ints), end(ints), CustomSort());
        std::sort(ints.begin(), ints.end(), CustomSort());

        int $match = 0;
        for(std::vector<int>::size_type i = 0; i < size; i++) {

            int $priority = std::get<0>(ints[i]);

            regex $uid = std::get<1>(ints[i]);
             // if we are below the count
             if((size - i) <= count && $match < count){
                 $match++;
                 ret.push_back($uid);
             }
             else if($priority <= priority && $match < count){
                $match++;
                ret.push_back($uid);
             }

        }

        return ret;
}


bool MakeMatch(std::string line, vector<regex> regexes){

   bool ret = false;

   for(std::vector<int>::size_type i = 0; i < regexes.size(); i++) {

      std::smatch match;
      regex r = regexes[i];

      if (std::regex_search(line, match, r) && match.size() > 0) {
            return true;
       }

    }


    return ret;
}



void Method(const v8::FunctionCallbackInfo<v8::Value>& args) {

    if (args.Length() < 2) {
        Nan::ThrowTypeError("Wrong number of arguments");
        return;
    }

    if (!args[0]->IsString()) {
        Nan::ThrowTypeError("First argument to 'replace-line' must be the absolute file path.");
        return;
    }

    //infile.open("/home/oleg/dogs.txt", fstream::in | fstream::out);
    //f.open("/home/oleg/dogs.txt", fstream::in);
    v8::String::Utf8Value filepath(args[0]->ToString());


    //std::regex reg1;

//    if (!args[1]->IsString()) {
//        Nan::ThrowTypeError("Second argument to 'replace-line' must be a string regular expression.");
//        return;
//    }


      v8::Local<v8::Array> input = v8::Local<v8::Array>::Cast(args[1]);

      vector<regex> inputRegexes;
      int len = input->Length();

      for(std::vector<int>::size_type i = 0; i < len; i++) {

         v8::String::Utf8Value target(input->Get(i));
         std::string trgt(*target);
         std::regex reg1(trgt);
         inputRegexes.push_back(reg1);

       }


//     v8::String::Utf8Value target(args[1]);
//     std::string trgt(*target);
//     std::regex reg1(trgt);


    v8::String::Utf8Value arg2(args[2]->ToString());
    if (strcmp(*arg2, "true") == 0 && strcmp(*arg2, "false") == 0) {
        Nan::ThrowTypeError("Third argument to 'replace-line' must be a string representing whether to find or replace.");
        return;
    }

    //    int count = args[2]->ToNumber()->NumberValue();
    //    int priority = args[3]->ToNumber()->NumberValue();
    //    int priority_search_cap = args[4]->ToNumber()->NumberValue();

    bool isReplace = strcmp(*arg2, "true") == 0 ? true : false;
//     cout << " isReplace " << isReplace << endl;

    int count = args[3]->IsUndefined() ? 1 : args[3]->ToNumber()->NumberValue();
//     cout << " count " << count << endl;

    int priority = args[4]->IsUndefined() ? 0 : args[4]->ToNumber()->NumberValue();
//     cout << " priority " << priority << endl;

    int priority_search_cap = args[5]->IsUndefined() ? 20 : args[5]->ToNumber()->NumberValue();
//    cout << " priority_search_cap " << priority_search_cap << endl;

    //    int priority = args[3]->ToNumber()->NumberValue();
    //    int priority_search_cap = args[4]->ToNumber()->NumberValue();
    //
    //    cout << " count => " << count << endl;
    //    cout << " priority => " << priority << endl;
    //    cout << " priority_search_cap => " << priority_search_cap << endl;

    //  double arg0 = info[0]->NumberValue();
    //  double arg1 = info[1]->NumberValue();
    //  v8::Local<v8::Number> num = Nan::New(arg0 + arg1);

    fstream f;
    int index = 0;


    if( priority > 0 ) {

        f.open(*filepath, fstream::in);

        if(f.fail()) {
            cerr << " => 'replace-line' usage error =>  infile fail => path to file was likely invalid. " << endl;
            exit(1);
        }

        if(!f.is_open()) {
            cerr << " file could not be opened " << endl;
            exit(1);
        }
        vector<string> priorityMatches;
        string line;
        while (getline(f, line) && index < priority_search_cap) {

            string str(line);
            std::smatch match;

//            if (std::regex_search(str, match, reg1) && match.size() > 0) {

        if (MakeMatch(str, inputRegexes)) {
                index++;
                priorityMatches.push_back(str);
            }

        }

        //int priority, vector<string> v, int count, int numberOfRecursiveCalls

        inputRegexes = FindPriorityItems(priority, priorityMatches, count, 0);
        f.close();

    }


    f.open(*filepath, fstream::in);

    if(f.fail()) {
        cerr << " => 'replace-line' usage error =>  infile fail => path to file was likely invalid. " << endl;
        exit(1);
    }

    if(!f.is_open()) {
        cerr << " file could not be opened " << endl;
        exit(1);
    }

    vector<string> v;
    vector<vector<int>> ints;

    string line;
    index = 0;

    while (getline(f, line) && index < count) {

        string str(line);
        std::smatch match;

        long old_offset = f.tellp()-str.length() -1;

//        if (std::regex_search(str, match, reg1) && match.size() > 0) {

          if(MakeMatch(str,inputRegexes)){

            index++;
            long new_offset = f.tellp();

            v.push_back(str);

            if(old_offset > 1) {
                old_offset--;
            }

            long len = new_offset - old_offset - 1;
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

        if(isReplace) {

            int pos = ints[i][0];
            int len = ints[i][1];

            z.seekp (pos);
            string s(len, ' ');
            //     s = "\n" + s;
            z << s;

        }


    }


    z.close();

    if(isReplace) {

        fstream x;
        x.open(*filepath, fstream::in);

        int ind = 0;

        int off = -1;
        int leng = -1;

        while (getline(x, line) && ind < 1) {

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


        if(off > -1) {

//  cout << " off is on " << endl;

            fstream m;
            m.open(*filepath);

            m.seekp (off);
            string xx(leng + 1, ' ');
//     s = "\n" + s;
            m << xx;

            m.close();
        }

    } // end if isReplace



    args.GetReturnValue().Set(a);

}

void init(v8::Local<v8::Object> exports) {
    NODE_SET_METHOD(exports, "hello", Method);
}

NODE_MODULE(hello_addon, init)
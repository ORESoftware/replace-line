
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

///////////////////////////////////////////////////////////////////////////////////////////////

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

       int sze = v.size();

        for(std::vector<int>::size_type i = 0; ((unsigned)i) < ((unsigned)sze); i++) {
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
        for(std::vector<int>::size_type i = 0; ((unsigned)i) < ((unsigned)size); i++) {

            int $priority = std::get<0>(ints[i]);

            regex $uid = std::get<1>(ints[i]);
             // if we are below the count
             if((size - ((unsigned)i)) <= ((unsigned)count) && ((unsigned)$match) < ((unsigned)count)){
                 $match++;
                 ret.push_back($uid);
             }
             else if($priority <= priority && ((unsigned)$match) < ((unsigned)count)){
                $match++;
                ret.push_back($uid);
             }

        }

        return ret;
}


bool MakeMatch(std::string line, vector<regex> regexes){

   for(std::vector<int>::size_type i = 0; ((unsigned)i) < regexes.size(); i++) {

      std::smatch match;
      regex r = regexes[i];

      if (std::regex_search(line, match, r) && match.size() > 0) {
            return true;
       }

    }

    return false;
}



void run(const v8::FunctionCallbackInfo<v8::Value>& args) {

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

      for(std::vector<int>::size_type i = 0; ((unsigned)i) < ((unsigned)len); i++) {

         v8::String::Utf8Value target(input->Get(i));
         std::string trgt(*target);
         std::regex reg1(trgt);
         inputRegexes.push_back(reg1);

       }


    v8::String::Utf8Value arg2(args[2]->ToString());
    if (strcmp(*arg2, "true") == 0 && strcmp(*arg2, "false") == 0) {
        Nan::ThrowTypeError("Third argument to 'replace-line' must be a string representing whether to find or replace.");
        return;
    }


    bool isReplace = strcmp(*arg2, "true") == 0 ? true : false;
//     cout << " isReplace " << isReplace << endl;

    int count = args[3]->IsUndefined() ? 1 : args[3]->NumberValue();
//     cout << " count " << count << endl;

    int priority = args[4]->IsUndefined() ? 0 : args[4]->NumberValue();
//     cout << " priority " << priority << endl;

    int priority_search_cap = args[5]->IsUndefined() ? 20 : args[5]->NumberValue();

    //  psc should be as large as count if count is bigger (should not happen often)
    priority_search_cap = std::max(count,priority_search_cap);


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
        while (getline(f, line) && ((unsigned)index) < ((unsigned)priority_search_cap)) {

            string str(line);
            std::smatch match;

        if (MakeMatch(str, inputRegexes)) {
                index++;
                priorityMatches.push_back(str);
            }

        }

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

    while (getline(f, line) && ((unsigned)index) < ((unsigned)count)) {

        string str(line);
        std::smatch match;

        int ln = str.length();
        int tp = f.tellp();

        int old_offset =  tp - ln -1;

          if(MakeMatch(str,inputRegexes)){

            index++;
            int new_offset = f.tellp();

            v.push_back(str);

            if(old_offset > 1) {
                old_offset--;
            }

            int len = new_offset - old_offset - 1;
            vector<int> c = {old_offset,len};
            ints.push_back(c);
        }

    }

    f.close();

    fstream z;
    z.open(*filepath);


    v8::Isolate* isolate = args.GetIsolate();
    v8::Local<v8::Array> a = v8::Array::New(isolate);

    int sze = v.size();

    for(std::vector<int>::size_type i = 0; ((unsigned)i) < ((unsigned)sze); i++) {

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

        while (getline(x, line) && ((unsigned)ind) < 1) {

            string str(line);

            char h = str[0];
            char p = str[str.length()-1];

//       cout << " p " << p << endl;

            bool hIsChar = std::isspace(static_cast<unsigned char>(h));
            bool pIsChar = std::isspace(static_cast<unsigned char>(p));

            if (hIsChar && !pIsChar) {
                int tel = x.tellp();
                leng = str.length();
                off =  tel - leng;

            }

        }

        x.close();


        if(off > -1) {


            fstream m;
            m.open(*filepath);

            m.seekp (off);
            string xx(leng + 1, ' ');

            m << xx;

            m.close();
        }

    } // end if isReplace



    args.GetReturnValue().Set(a);

}

void init(v8::Local<v8::Object> exports) {
    NODE_SET_METHOD(exports, "run", run);
}

NODE_MODULE(run, init)
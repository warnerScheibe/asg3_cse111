// $Id: main.cpp,v 1.11 2018-01-25 14:19:29-08 - - $

#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <unistd.h>
#include <regex>
#include <cassert>

using namespace std;

#include "listmap.h"
#include "xpair.h"
#include "util.h"

using str_str_map = listmap<string,string>;
using str_str_pair = str_str_map::value_type;

void scan_options (int argc, char** argv) {
   opterr = 0;
   for (;;) {
      int option = getopt (argc, argv, "@:");
      if (option == EOF) break;
      switch (option) {
         case '@':
            debugflags::setflags (optarg);
            break;
         default:
            complain() << "-" << char (optopt) << ": invalid option"
                       << endl;
            break;
      }
   }
}

int main (int argc, char** argv) {
   sys_info::execname (argv[0]);
   scan_options (argc, argv);

   
   str_str_map test;
   regex comment_regex {R"(^\s*(#.*)?$)"};
   regex key_value_regex {R"(^\s*(.*?)\s*=\s*(.*?)\s*$)"};
   regex key_equals_regex {R"(^\s*(.*)\s*=\s*$)"};
   regex equals_value_regex {R"(^\s*[=]\s*(.*?)\s*$)"};
   regex trimmed_regex {R"(^\s*([^=]+?)\s*$)"};
    
   for (;;) {
      string line;
      getline (cin, line);
      if (cin.eof()) break;
      cout << endl << "input: \"" << line << "\"" << endl;
      smatch result;
      if (regex_search (line, result, comment_regex)) {
         cout << "Comment or empty line." << endl;
         continue;
      }
      //if(line.find("=") == string::npos &&
      //   line.find(" ") != string::npos){
      //   throw "line ignored";
      //   continue;
      //}
       
      else if(line.at(0) == '=' &&
          line.size() == 1){
           if(!test.empty()){
               test.print_list(); //need to call this more than once or make 2 print functions
           }
           else{
               cout << "map is empty " << endl;
           }
           continue;
       }
      else if (regex_search (line, result, key_equals_regex))
      {
          test.erase(test.find(result[1]));
      }
      else if (regex_search (line, result, equals_value_regex)) {
		  auto it = test.begin();
		  for ( ; it != test.end(); ++it)
		  {
			  if(result[1] == it -> second)
			  {
				  test.print_pair(*it);
		      }
	      }
      }
      else if (regex_search (line, result, key_value_regex)) {
          cout << "key  : \"" << result[1] << "\"" << endl;
          cout << "value: \"" << result[2] << "\"" << endl;
          str_str_pair pair (result[1], result[2]); //declared here to make sure no errors
          test.insert(pair);
          //test.find(result[1]);
          test.print_pair(pair);
      }
      else if (regex_search (line, result, trimmed_regex)) {
         cout << "query: \"" << result[1] << "\"" << endl;
         test.find(result[1]);
      }
      else {
         assert (false and "This can not happen.");
      }
   }
   return 0;
    
    
}

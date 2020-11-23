// $Id: main.cpp,v 1.11 2018-01-25 14:19:29-08 - - $
// Warner Scheibe   (wscheibe@ucsc.edu)
// Alain Kassarjian (akassarj@ucsc.edu)

#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <unistd.h>
#include <regex>
#include <cassert>
#include <fstream>

using namespace std;

#include "listmap.h"
#include "xpair.h"
#include "util.h"

using str_str_map = listmap<string,string>;
using str_str_pair = str_str_map::value_type;
fstream infile;
string filename;

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

   if (argc > 1)
   {
       infile.open(argv[1]);
       if(infile.fail())
       {
          return 1;
       }
       filename = argv[1];
   }
   else
   {
       filename = "-";
   }
   
   str_str_map test;
   int command_number = 0;
   regex comment_regex {R"(^\s*(#.*)?$)"};
   regex key_value_regex {R"(^\s*(.*?)\s*=\s*(.*?)\s*$)"};
   regex key_equals_regex {R"(^\s*(.*)\s*=\s*$)"};
   regex equals_value_regex {R"(^\s*[=]\s*(.*?)\s*$)"};
   regex trimmed_regex {R"(^\s*([^=]+?)\s*$)"};
   regex key_regex {R"(^\s*(.*)\s*?$)"};
    
   for (;;) {
      string line;
      command_number++;
      if(infile.is_open())
      {
         getline (infile, line);
         if (infile.eof()) break;
      }
      else
      {
          getline(cin, line);
          if (cin.eof()) break;
      }
      cout <<filename<< ": " << command_number << ": " << line << endl;
      smatch result;
      if (regex_search (line, result, comment_regex)) {
         continue;
      }
       
      else if(line.at(0) == '=' &&
          line.size() == 1){
           if(!test.empty()){
               test.print_list(); 
           }
           continue;
       }
      else if (regex_search (line, result, key_equals_regex))
      {
          test.erase(test.find(result[1]));
      }
      else if (regex_search (line, result,
                             equals_value_regex)) {
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
          //declared here to make sure no errors
          str_str_pair pair (result[1], result[2]);
          test.insert(pair);
          test.print_pair(pair);
      }
      else if (regex_search (line, result, key_regex))
      {
          if(test.find(result[1]) != test.end())
          {
             test.print_pair(*test.find(result[1]));
          }
          else
          {
             cout << result[1] << ": key not found" << endl;
          }
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

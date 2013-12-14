#ifndef BASE_INSTRUMENTER_H_
#define BASE_INSTRUMENTER_H_

#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <sys/types.h>

using namespace std;

class instrumenter
{
 public: 
  instrumenter(); 
  instrumenter(string log_file_dir,
	       string log_file_name_prefix,
	       string log_file_name_extension);
  ~instrumenter();  // Not implemented yet
  
  bool log_instrument_data(string log_level, string tag, 
			   string log_message, pid_t pid = -1,
			   string curr_time = ""); 

  template<class T>
    string stringify(T val)
    {
      ostringstream o;
      o << val;
      return o.str();
    }

  template<class T>
    const char * c_stringify(T val)
    {
      string ret = stringify(val);
      return ret.c_str();
    } 

  inline string get_mime_type(string key)
  {
    return this->mime_type_map[key];
  }

  string build_xml_element(string, string, bool); 

  void populate_mime_type_map();

  string get_time();

 private:
  string log_file_dir;
  string log_file_name_prefix;
  string log_file_name_extension;
  map<string, string> mime_type_map;
};


#endif

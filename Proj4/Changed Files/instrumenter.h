#ifndef BASE_INSTRUMENTER_H_
#define BASE_INSTRUMENTER_H_

#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <sys/types.h>

#include "base/files/file_path.h"
#include "base/time/time.h"
#include "instrumenter_database.h"
#include "url/gurl.h"

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

  string get_time(time_t time = 0);
  void collect_and_log_download_data(GURL, GURL, base::FilePath, string, int64);
  void collect_and_log_touch_data(int, int, int);
  void collect_and_log_cookie_data(GURL, string, string, base::Time);
  void collect_and_log_cookie_line(GURL, string);

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

 private:
  void populate_mime_type_map();
  string log_file_dir;
  string log_file_name_prefix;
  string log_file_name_extension;
  map<string, string> mime_type_map;
  static string touch_state_map[];
};


#endif

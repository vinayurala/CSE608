#ifndef BASE_INSTRUMENTER_DATABASE_H
#define BASE_INSTRUMENTER_DATABASE_H

#include <string>
#include <sys/types.h>
#include <iostream>
#include <sstream>

#include "base/files/file_path.h"
#include "sql/meta_table.h"
#include "sql/connection.h"

using namespace std;

/**
---------------InstrumenterDatabase schema-----------------
Event ID counter - A counter that starts from 1 
                   (Primary key)
Time             - Time im YY-MM-DD HH:mm:ss
Tag              - Tag specifying what component triggered 
                   instrumentation
Message          - Logging message

A per process table will store the above data. 
-----------------------------------------------------------
*/ 
class InstrumenterDatabase {

  sql::Connection db_;
  base::FilePath db_file_name_;
  base::FilePath db_dir_name_;
  bool is_db_created_;

  static InstrumenterDatabase *db_instance_;
  InstrumenterDatabase();
  InstrumenterDatabase(string);
  InstrumenterDatabase(string, string);

  bool create_file(const char*);
  bool check_and_create_table(string);

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

 public: 
  ~InstrumenterDatabase();
  bool init();
  void uninit();
  bool add_row(string, string, string, string, pid_t);
  bool delete_row(int);
  bool delete_all_rows(pid_t);
  static InstrumenterDatabase* get_db_instance();
};

#endif

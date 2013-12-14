#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <cerrno>
#include <fcntl.h>
#include <sys/stat.h>

#include "base/instrumenter_database.h"

using namespace std;

InstrumenterDatabase* InstrumenterDatabase::db_instance_ = NULL;

InstrumenterDatabase::InstrumenterDatabase()
{
  db_file_name_ = base::FilePath("InstrumentedEvents.db");
#if defined(OS_ANDROID)
  db_dir_name_ = base::FilePath("/sdcard/");
#else
  db_dir_name_ = base::FilePath("/var/log/");
#endif
  is_db_created_ = false;
}

InstrumenterDatabase::InstrumenterDatabase(string db_name)
{
  db_dir_name_ = base::FilePath("/sdcard/");
  db_file_name_ = base::FilePath(db_name);
  is_db_created_ = false;
}

InstrumenterDatabase::InstrumenterDatabase(string db_file_dir, 
					   string db_name)
{
  db_dir_name_ = base::FilePath(db_file_dir);
  db_file_name_ = base::FilePath(db_name);
  is_db_created_ = false;
}

InstrumenterDatabase::~InstrumenterDatabase()
{
  uninit();
}

InstrumenterDatabase* InstrumenterDatabase::get_db_instance()
{
  if (!db_instance_)
    db_instance_ = new InstrumenterDatabase;
  
  return db_instance_;
}

bool InstrumenterDatabase::init()
{
  string db_file = db_dir_name_.value() + db_file_name_.value();

  if (is_db_created_)
    return db_.Open(base::FilePath(db_file));

  // check if db_file_name_ exists, else create it
  if (!create_file(db_file.c_str()))
    return false;
  is_db_created_ = true;

  db_.set_exclusive_locking();
  return db_.Open(base::FilePath(db_file));
}

void InstrumenterDatabase::uninit()
{
  if (db_.is_open())
    db_.Close();
}

bool InstrumenterDatabase::create_file(const char *path)
{
  if (!access(path, F_OK))
    return true;

  int fd = open(path, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
  if (fd < 0)
    return false;
  
  close(fd);
  return true;
}

bool InstrumenterDatabase::check_and_create_table(string table_name)
{
  string query;

  if (!db_.DoesTableExist(table_name.c_str())) {
    query = "CREATE TABLE " + table_name + " (id INTEGER PRIMARY KEY, "
      "time VARCHAR, tag VARCHAR, log_message VARCHAR)";
    return db_.Execute(query.c_str());
  }
  return true;
}

bool InstrumenterDatabase::add_row(string proc_name, string time,
				   string tag, string log_msg, pid_t pid)
{
  string table_name = proc_name + stringify(pid);

  init();
  if (!(check_and_create_table(table_name)))
    return false;

  static int event_id = 0;
  event_id += 1;
  string event_id_str = string(stringify(event_id));
  string query = "INSERT INTO \"" + table_name + "\" VALUES (" + 
    event_id_str + ", \"" + time + "\", \"" + 
    tag + "\", \"" + log_msg + "\")";
  
  return db_.Execute(query.c_str());
  uninit();
}
				
bool InstrumenterDatabase::delete_all_rows(pid_t pid)
{
  string table_name = stringify(pid);
  string query = "DELETE FROM \"" + table_name + "\"";
  
  return db_.Execute(query.c_str());
}

bool InstrumenterDatabase::delete_row(int key)
{
  return false;
}

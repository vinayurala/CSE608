#include <string>
#include <ctime>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#include "instrumenter_message_filter.h"
#include "base/logging.h"

using namespace std;

namespace content {
  
  InstrumenterMessageFilter::InstrumenterMessageFilter()
  {
#if defined(OS_ANDROID)
    file_dir_ = "/sdcard/File contents/";
#else
    file_dir_ = "/var/log/File contents/";
#endif
    check_and_create_dir(file_dir_.c_str());
  }
  
  InstrumenterMessageFilter::InstrumenterMessageFilter(string file_dir) 
  {
    file_dir_ = file_dir;
  }

  InstrumenterMessageFilter::~InstrumenterMessageFilter()
  {
  }
  
  void InstrumenterMessageFilter::check_and_create_dir(const char *file_dir)
  {
    if (!access(file_dir, F_OK))
      return;

     mkdir(file_dir, S_IRWXU | S_IRWXG | S_IRWXO);

    return;
  }

  bool InstrumenterMessageFilter::OnMessageReceived(const IPC::Message& message,
						    bool *message_was_ok)
  {
    bool handled = true;
    IPC_BEGIN_MESSAGE_MAP_EX(InstrumenterMessageFilter, message, *message_was_ok)
      IPC_MESSAGE_HANDLER(RenderMessage, log_data)
      IPC_MESSAGE_HANDLER(RenderInitFile, create_html_content_file)
      IPC_MESSAGE_HANDLER(RenderWriteToFile, write_html_content)
      IPC_MESSAGE_UNHANDLED(handled = false)
    IPC_END_MESSAGE_MAP()
      
    return handled;
  }
  
  void InstrumenterMessageFilter::log_data(string log_level, string tag,
					   string log_msg, pid_t pid,
					   string curr_time)
  {
    i_obj_.log_instrument_data(log_level, tag,
			      log_msg, pid,
			      curr_time);
  }

  string InstrumenterMessageFilter::create_html_content_file(pid_t pid, bool is_html_content)
  {
    time_t t = time(0);

    if (is_html_content)
      file_subdir_ = "HTML Contents/";
    else
      file_subdir_ = "Script Contents/";

    check_and_create_dir((file_dir_ + file_subdir_).c_str());
    
    content_file_ = file_dir_ + file_subdir_ + i_obj_.stringify(t) + ".txt";
    file_stream_.open(content_file_.c_str(), ios::out | ios::app);

    file_stream_ << "---------------File content captured by instrumenter------------------\n";

    string log_msg = "File content will be logged onto file " + content_file_;
    if (is_html_content)
      i_obj_.log_instrument_data("verbose", "HTML content", log_msg, pid);
    else
      i_obj_.log_instrument_data("verbose", "Script content", log_msg, pid);

    return content_file_;
  }

  void InstrumenterMessageFilter::write_html_content(string data, bool end_of_data_stream)
  {
    file_stream_ << data;
    if (end_of_data_stream)
      file_stream_.close();
  }
}

#ifndef CONTENT_BROWSER_RENDERER_HOST_INSTRUMENTER_IPC_LISTENER_H
#define CONTENT_BROWSER_RENDERER_HOST_INSTRUMENTER_IPC_LISTENER_H

#include "base/instrumenter.h"
#include "ipc/ipc_message_macros.h"
#include "content/public/browser/browser_message_filter.h"
#include "content/common/renderer_instrumenter_messages.h"

#include <string>
#include <sys/types.h>
#include <fstream>
#include <iostream>

namespace content {

  class InstrumenterMessageFilter : public BrowserMessageFilter
  {
    instrumenter i_obj_;
    ofstream file_stream_;
    string content_file_;
    string file_dir_;
    string file_subdir_;

    void check_and_create_dir(const char *);

  public:
    InstrumenterMessageFilter();
    InstrumenterMessageFilter(string);
    ~InstrumenterMessageFilter();
    
    bool OnMessageReceived(const IPC::Message& msg, 
			   bool *message_was_ok);
    void log_data(string, string, string,
		  pid_t, string);

    string create_html_content_file(pid_t, bool);
    void write_html_content(string, bool);
  };
}
#endif

#ifndef CONTENT_BROWSER_RENDERER_HOST_INSTRUMENTER_IPC_LISTENER_H
#define CONTENT_BROWSER_RENDERER_HOST_INSTRUMENTER_IPC_LISTENER_H

#include "base/instrumenter.h"
#include "ipc/ipc_message_macros.h"
#include "content/public/browser/browser_message_filter.h"
#include "content/common/renderer_instrumenter_messages.h"

#include <string>
#include <sys/types.h>

namespace content {

  class instrumenter_ipc_listener : public BrowserMessageFilter
  {
    instrumenter_ipc_listener();
    ~instrumenter_ipc_listener();
    
    bool OnMessageReceived(const IPC::Message& msg, bool *message_was_ok);
    void log_data(string,
		  string,
		  string,
		  pid_t,
		  string);
  };
}
#endif

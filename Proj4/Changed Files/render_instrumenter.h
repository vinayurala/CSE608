#ifndef BASE_RENDERER_INSTRUMENTER_H
#define BASE_RENDERER_INSTRUMENTER_H

#include "content/common/renderer_instrumenter_messages.h"
#include "content/renderer/render_thread_impl.h"
#include "ipc/ipc_message.h"
#include "ipc/ipc_channel_proxy.h"
#include "ipc/ipc_sender.h"

#include <sys/types.h>
#include <string>

using namespace std;

class render_instrumenter 
{
 public:
  render_instrumenter();
  ~render_instrumenter();
  
  bool dispatch_message(string log_level, string tag, string log_msg);
  int generate_routing_id();
  bool write_content(string data, bool end_of_data_stream);
  bool init_content_file(bool);

 private:
   pid_t pid_;
   int routing_id_;
};

#endif

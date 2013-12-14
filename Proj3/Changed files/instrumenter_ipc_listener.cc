#include <string>

#include "instrumenter_ipc_listener.h"
#include "base/logging.h"

using namespace std;

namespace content {
  
  instrumenter_ipc_listener::instrumenter_ipc_listener()
  {
  }
  
  instrumenter_ipc_listener::~instrumenter_ipc_listener()
  {
  }
  
  bool instrumenter_ipc_listener::OnMessageReceived(const IPC::Message& message,
						    bool *message_was_ok)
  {
    LOG (DFATAL) << "Received message (browser)";
    bool handled = true;
    IPC_BEGIN_MESSAGE_MAP_EX(instrumenter_ipc_listener, message, *message_was_ok)
      IPC_MESSAGE_HANDLER(RenderMessage,
			  log_data)
      IPC_MESSAGE_UNHANDLED(handled = false)
      IPC_END_MESSAGE_MAP()
      
      return handled;
  }
  
  void instrumenter_ipc_listener::log_data(string log_level, string tag,
					   string log_msg, pid_t pid,
					   string curr_time)
  {
    LOG (DFATAL) << "Logging data (browser)";
    instrumenter i_obj;
    i_obj.log_instrument_data(log_level, tag,
			      log_msg, pid,
			      curr_time);
  }
}

#include <string>
#include <unistd.h>

#include "render_instrumenter.h"
#include "base/instrumenter.h"

using namespace std;

render_instrumenter::render_instrumenter()
{
  pid_ = getpid();
}

render_instrumenter::~render_instrumenter()
{
}

bool render_instrumenter::dispatch_message(string log_level, string tag, string  log_msg)
{
  if (log_level.empty() || tag.empty() || log_msg.empty()) {
    LOG (DFATAL) << "Cannot log render instrumentation data; missing data";
    return false;
  }

  instrumenter i_obj;
  string curr_time = i_obj.get_time();

  LOG (DFATAL) << "Sending new IPC request";
  content::RenderThreadImpl::current()->Send(new RenderMessage(log_level,
							       tag, log_msg, 
							       this->pid_, curr_time));
  return true;
}

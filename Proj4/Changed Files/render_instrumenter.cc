#include <string>
#include <unistd.h>

#include "render_instrumenter.h"

#include "base/instrumenter.h"
#include "base/basictypes.h"
#include "ipc/ipc_message.h"

using namespace std;

render_instrumenter::render_instrumenter()
{
  pid_ = getpid();
}

render_instrumenter::~render_instrumenter()
{
}

int render_instrumenter::generate_routing_id()
{
  static int last_id = 0;
  last_id++;
  
  // skip over special routing ids
  if (last_id == -2 || last_id == kint32max)
    last_id++;

  return last_id;

}

bool render_instrumenter::dispatch_message(string log_level, string tag, string  log_msg)
{
  if (log_level.empty() || tag.empty() || log_msg.empty()) {
    LOG (DFATAL) << "Cannot log render instrumentation data; missing data";
    return false;
  }

  instrumenter i_obj;
  string curr_time = i_obj.get_time();

  routing_id_ = generate_routing_id();
  content::RenderThreadImpl::current()->Send(new RenderMessage(routing_id_, log_level,
							       tag, log_msg, 
							       this->pid_, curr_time));

  return true;
}

bool render_instrumenter::init_content_file(bool is_html_content)
{
  routing_id_ = generate_routing_id();
  content::RenderThreadImpl::current()->Send(new RenderInitFile(routing_id_,
								pid_, is_html_content));
  return true;

}

bool render_instrumenter::write_content(string data, bool end_of_stream)
{
  routing_id_ = generate_routing_id();

  content::RenderThreadImpl::current()->Send(new RenderWriteToFile(routing_id_,
								   data, 
								   end_of_stream));
  
  return true;
}

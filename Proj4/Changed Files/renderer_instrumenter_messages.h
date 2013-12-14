#include <string>
#include <sys/types.h>

#include "ipc/ipc_message_macros.h"
#include "ipc/ipc_message_start.h"

#define IPC_MESSAGE_START RendererInstrumenterStart

#ifndef CONTENT_COMMON_RENDERER_INSTRUMENTER_MESSAGES_H
#define CONTENT_COMMON_RENDERER_INSTRUMENTER_MESSAGES_H

IPC_MESSAGE_ROUTED5(RenderMessage,
		    std::string, /* Log Level */
		    std::string, /* Tag */
		    std::string, /* Log message */
		    pid_t,       /* PID */
		    std::string  /* Timestamp */ );

IPC_MESSAGE_ROUTED2(RenderInitFile,
		    pid_t, /* PID */
		    bool   /* HTML or Script content */);

IPC_MESSAGE_ROUTED2(RenderWriteToFile,
		    std::string, /* Data to write */ 
		    bool         /* End of data stream */ );

#endif

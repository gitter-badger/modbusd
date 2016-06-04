//
// main.h
// taka-wang
//

#ifndef MAIN_H
#define MAIN_H

// marco
#define VERSION "0.0.2"
#define IPC_SUB "ipc:///tmp/to.modbus"
#define IPC_PUB "ipc:///tmp/from.modbus"

// function pointer
typedef void (*mbtcp_fc_fp)(mbtcp_handle_s **ptr_handle, cJSON **ptr_req);


#endif  // MAIN_H
#ifndef __TUYA_P2P_MODULE_UPNP_H__
#define __TUYA_P2P_MODULE_UPNP_H__

#include <stdio.h>

////////////////////////////////////////////////////////////////////////////////////////////

typedef enum{
	UPNP_MODULE_READY = 0,		
	UPNP_MODULE_RUNNING,			
	UPNP_MODULE_DONE				
}tuya_upnp_module_status;

typedef enum{
	UPNP_DEC = 0,
	UPNP_STRING
}tuya_upnp_log_seq_type;

typedef enum {
    UPNP_PORT_PROTOCOL_UDP,
    UPNP_PORT_PROTOCOL_TCP
}tuya_upnp_port_protocol;


typedef int (*TUYA_UPNP_CONFIG_REPORT_CB)(char *config);

typedef void (*TUYA_UPNP_LOG_SEQ_CB)(tuya_upnp_log_seq_type type, unsigned char id, void* log);

typedef struct tuya_upnp_module_cb{
	TUYA_UPNP_CONFIG_REPORT_CB report_cb;
	TUYA_UPNP_LOG_SEQ_CB log_seq_cb;
}tuya_upnp_module_cb;


////////////////////////////////////////////////////////////////////////////////////////////
int tuya_upnp_module_init(int session_num, unsigned char* request, tuya_upnp_module_cb module_cb);

void tuya_upnp_module_deinit();

int tuya_upnp_module_modify(int session_num, unsigned char* request);

int tuya_upnp_module_request_port(tuya_upnp_port_protocol protocol, int *local_port, char *ext_address, int *ext_port);

int tuya_upnp_module_release_port(tuya_upnp_port_protocol protocol, int local_port);

int tuya_upnp_module_bind_result(tuya_upnp_port_protocol protocol, int local_port, int error_code);

tuya_upnp_module_status tuya_upnp_module_get_status();
////////////////////////////////////////////////////////////////////////////////////////////

#endif

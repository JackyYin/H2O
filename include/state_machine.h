#ifndef _H2O_STATE_MACHINE_H
#define _H2O_STATE_MACHINE_H
#include "h2o.h"
typedef enum {
	SSM_EVENT_SEND_H,
	SSM_EVENT_RECV_H,
	SSM_EVENT_SEND_ES,
	SSM_EVENT_RECV_ES,
	SSM_EVENT_SEND_R,
	SSM_EVENT_RECV_R,
	SSM_EVENT_SEND_PP,
	SSM_EVENT_RECV_PP,
	SSM_EVENT_MAX
} h2o_ssm_event; // stream event

typedef enum {
	SSM_STATE_IDLE = H2O_STREAM_STATE_IDLE,
	SSM_STATE_RESERVED_LOCAL = H2O_STREAM_STATE_RESERVED_LOCAL,
	SSM_STATE_RESERVED_REMOTE = H2O_STREAM_STATE_RESERVED_REMOTE,
	SSM_STATE_OPEN = H2O_STREAM_STATE_OPEN,
	SSM_STATE_HALF_CLOSED_LOCAL = H2O_STREAM_STATE_HALF_CLOSED_LOCAL,
	SSM_STATE_HALF_CLOSED_REMOTE = H2O_STREAM_STATE_HALF_CLOSED_REMOTE,
	SSM_STATE_CLOSED = H2O_STREAM_STATE_CLOSED,
	SSM_STATE_NORMAL_MAX,
	SSM_STATE_ERR_PROTOCOL_ERROR,
	SSM_STATE_ERR_STREAM_CLOSED_ERROR,
	SSM_STATE_IGNORED,
	SSM_STATE_SHOULD_NOT_HAPPEN,
	SSM_STATE_MAX
} h2o_ssm_state;

struct stream_state_machine {
	uint8_t current_state;
	void *state_action_cb;
	void *state_action_args;
};
typedef struct stream_state_machine h2o_ssm;
typedef void (*h2o_ssm_state_action_cb)(h2o_ssm_state new_state, void *data);

void init_ssm(h2o_ssm *m);
void register_state_action_callback(h2o_ssm *m,
				    h2o_ssm_state_action_cb callback,
				    void *data);
int handle_external_event(h2o_ssm *m, h2o_ssm_event e);

int ssm_check_send_frame(h2o_ssm *m, enum h2o_frame_type type);
#endif

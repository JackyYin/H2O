#include "state_machine.h"
#include "h2o.h"
#include <errno.h>
#include <stddef.h>

h2o_ssm_state transision_map[][SSM_EVENT_MAX] = {
    [SSM_STATE_IDLE] = {[SSM_EVENT_SEND_H] = SSM_STATE_SHOULD_NOT_HAPPEN,
			[SSM_EVENT_RECV_H] = SSM_STATE_OPEN,
			[SSM_EVENT_SEND_ES] = SSM_STATE_IGNORED,
			[SSM_EVENT_RECV_ES] = SSM_STATE_IGNORED,
			[SSM_EVENT_SEND_R] = SSM_STATE_IGNORED,
			[SSM_EVENT_RECV_R] = SSM_STATE_ERR_PROTOCOL_ERROR,
			[SSM_EVENT_SEND_PP] = SSM_STATE_RESERVED_LOCAL,
			[SSM_EVENT_RECV_PP] = SSM_STATE_ERR_PROTOCOL_ERROR},
    [SSM_STATE_RESERVED_LOCAL] =
	{[SSM_EVENT_SEND_H] = SSM_STATE_HALF_CLOSED_LOCAL,
	 [SSM_EVENT_RECV_H] = SSM_STATE_ERR_PROTOCOL_ERROR,
	 [SSM_EVENT_SEND_ES] = SSM_STATE_IGNORED,
	 [SSM_EVENT_RECV_ES] = SSM_STATE_IGNORED,
	 [SSM_EVENT_SEND_R] = SSM_STATE_CLOSED,
	 [SSM_EVENT_RECV_R] = SSM_STATE_CLOSED,
	 [SSM_EVENT_SEND_PP] = SSM_STATE_SHOULD_NOT_HAPPEN,
	 [SSM_EVENT_RECV_PP] = SSM_STATE_ERR_PROTOCOL_ERROR},
    [SSM_STATE_RESERVED_REMOTE] =
	{[SSM_EVENT_SEND_H] = SSM_STATE_SHOULD_NOT_HAPPEN,
	 [SSM_EVENT_RECV_H] = SSM_STATE_HALF_CLOSED_LOCAL,
	 [SSM_EVENT_SEND_ES] = SSM_STATE_IGNORED,
	 [SSM_EVENT_RECV_ES] = SSM_STATE_IGNORED,
	 [SSM_EVENT_SEND_R] = SSM_STATE_CLOSED,
	 [SSM_EVENT_RECV_R] = SSM_STATE_CLOSED,
	 [SSM_EVENT_SEND_PP] = SSM_STATE_SHOULD_NOT_HAPPEN,
	 [SSM_EVENT_RECV_PP] = SSM_STATE_ERR_PROTOCOL_ERROR},
    [SSM_STATE_OPEN] = {[SSM_EVENT_SEND_H] = SSM_STATE_IGNORED,
			[SSM_EVENT_RECV_H] = SSM_STATE_IGNORED,
			[SSM_EVENT_SEND_ES] = SSM_STATE_HALF_CLOSED_LOCAL,
			[SSM_EVENT_RECV_ES] = SSM_STATE_HALF_CLOSED_REMOTE,
			[SSM_EVENT_SEND_R] = SSM_STATE_CLOSED,
			[SSM_EVENT_RECV_R] = SSM_STATE_CLOSED,
			[SSM_EVENT_SEND_PP] = SSM_STATE_IGNORED,
			[SSM_EVENT_RECV_PP] = SSM_STATE_IGNORED},
    [SSM_STATE_HALF_CLOSED_LOCAL] = {[SSM_EVENT_SEND_H] =
					 SSM_STATE_SHOULD_NOT_HAPPEN,
				     [SSM_EVENT_RECV_H] = SSM_STATE_IGNORED,
				     [SSM_EVENT_SEND_ES] = SSM_STATE_IGNORED,
				     [SSM_EVENT_RECV_ES] = SSM_STATE_CLOSED,
				     [SSM_EVENT_SEND_R] = SSM_STATE_CLOSED,
				     [SSM_EVENT_RECV_R] = SSM_STATE_CLOSED,
				     [SSM_EVENT_SEND_PP] =
					 SSM_STATE_SHOULD_NOT_HAPPEN,
				     [SSM_EVENT_RECV_PP] = SSM_STATE_IGNORED

    },
    [SSM_STATE_HALF_CLOSED_REMOTE] = {[SSM_EVENT_SEND_H] = SSM_STATE_IGNORED,
				      [SSM_EVENT_RECV_H] =
					  SSM_STATE_ERR_STREAM_CLOSED_ERROR,
				      [SSM_EVENT_SEND_ES] = SSM_STATE_CLOSED,
				      [SSM_EVENT_RECV_ES] = SSM_STATE_IGNORED,
				      [SSM_EVENT_SEND_R] = SSM_STATE_CLOSED,
				      [SSM_EVENT_RECV_R] = SSM_STATE_CLOSED,
				      [SSM_EVENT_SEND_PP] = SSM_STATE_IGNORED,
				      [SSM_EVENT_RECV_PP] =
					  SSM_STATE_ERR_STREAM_CLOSED_ERROR},
    [SSM_STATE_CLOSED] =
	{[SSM_EVENT_SEND_H] = SSM_STATE_SHOULD_NOT_HAPPEN,
	 [SSM_EVENT_RECV_H] = SSM_STATE_ERR_STREAM_CLOSED_ERROR,
	 [SSM_EVENT_SEND_ES] = SSM_STATE_IGNORED,
	 [SSM_EVENT_RECV_ES] = SSM_STATE_IGNORED,
	 [SSM_EVENT_SEND_R] = SSM_STATE_SHOULD_NOT_HAPPEN,
	 [SSM_EVENT_RECV_R] = SSM_STATE_ERR_STREAM_CLOSED_ERROR,
	 [SSM_EVENT_SEND_PP] = SSM_STATE_SHOULD_NOT_HAPPEN,
	 [SSM_EVENT_RECV_PP] = SSM_STATE_ERR_STREAM_CLOSED_ERROR},
};

void init_ssm(h2o_ssm *m)
{
	m->current_state = SSM_STATE_IDLE;
	m->state_action_cb = NULL;
	m->state_action_args = NULL;
}

void register_state_action_callback(h2o_ssm *m,
				    h2o_ssm_state_action_cb callback,
				    void *data)
{
	m->state_action_cb = (void *)callback;
	m->state_action_args = data;
}

int handle_external_event(h2o_ssm *m, h2o_ssm_event e)
{
	h2o_ssm_state new_state = transision_map[m->current_state][e];

	if (new_state == SSM_STATE_IGNORED)
		goto RETURN;

	if (m->state_action_cb)
		((h2o_ssm_state_action_cb)m->state_action_cb)(
		    new_state, m->state_action_args);

	if (new_state < SSM_STATE_NORMAL_MAX)
		m->current_state = new_state;
RETURN:
	return new_state;
}

int ssm_check_send_frame(h2o_ssm *m, enum h2o_frame_type type)
{
	if (m->current_state == SSM_STATE_IDLE) {
		if (type != H2O_FRAME_TYPE_PUSH_PROMISE)
			goto ERR;
	} else if (m->current_state == SSM_STATE_RESERVED_LOCAL) {
		if (type != H2O_FRAME_TYPE_HEADERS &&
		    type != H2O_FRAME_TYPE_RST_STREAM &&
		    type != H2O_FRAME_TYPE_PRIORITY)
			goto ERR;
	} else if (m->current_state == SSM_STATE_RESERVED_REMOTE ||
		   m->current_state == SSM_STATE_HALF_CLOSED_LOCAL) {
		if (type != H2O_FRAME_TYPE_RST_STREAM &&
		    type != H2O_FRAME_TYPE_WINDOW_UPDATE &&
		    type != H2O_FRAME_TYPE_PRIORITY)
			goto ERR;
	} else if (m->current_state == SSM_STATE_CLOSED) {
		if (type != H2O_FRAME_TYPE_PRIORITY)
			goto ERR;
	}
	return 0;
ERR:
	return -EPERM;
}

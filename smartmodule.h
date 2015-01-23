#ifndef SMARTMODULE_H_
#define SMARTMODULE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <mosquitto.h>

typedef void (*message_callback) (void *, const struct mosquitto_message *);

struct smartmodule_arguments 
{
	int 	hostport;
	char	*hostIP;
	char	*topic;
	message_callback message_cb;
};

/* These functions return ERROR value as an integer */
int smartmodule_init 	(struct smartmodule_arguments *arg);
int smartmodule_stop 	(struct smartmodule_arguments *arg);
int smartmodule_publish	(char *, int);

#ifdef __cplusplus
}
#endif

#endif /* SMARTMODULE_H_ */

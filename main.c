/* ============================================================================
 * @File: 	 main.c
 * @Author: 	 Ozgur Eralp [ozgur.eralp@outlook.com]
 * @Description: TEST Application running on ARM Platforms
 *
 * ============================================================================
 *
 * Copyright 2015 Ozgur Eralp.
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE COPYRIGHT HOLDERS, AUTHORS AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * ============================================================================
 *
 * THIS IS A TEST APPLICATION TO DEMO HOW EASY TO MAKE IT SMART
 * ASSUMPTIONS:
 *	- Mosquitto installed in the platform.
 *	- The Broker running and accessible on the network.
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <asm/errno.h>
#include "smartmodule.h"

void * t_smartmodule (void *arg);
void * t_keyreceive  (void *arg);

void cb_receivedmessage (void *, const struct mosquitto_message *);

int main(int argc, char *argv[])
{
	pthread_t tid1, tid2;
	int err;

	/* Initialize SmartModule */
	struct smartmodule_arguments   module;
	module.hostport   = 1883;
	module.hostIP     = "localhost";
	module.topic 	  = "hello/smarts";
	module.message_cb = (message_callback) &cb_receivedmessage;

	/* Initiate Threads */
	err = pthread_create(&tid1, NULL, &t_smartmodule, (void *)&module);
	err = pthread_create(&tid2, NULL, &t_keyreceive,  (void *)&module);

	err = pthread_join(tid1, NULL);
	err = pthread_join(tid2, NULL);

	if (err != 0)
		printf("..$$ ERROR occured during thread initialization..\n");

	printf("\n.Good Bye!.\n\n");

	return 0;
}

/* ============================================================================
 * @Function: 	 cb_receivedmessage
 * @Description: Received Message from Network via Broker.
 * ============================================================================
 */
void cb_receivedmessage (void *userdata, const struct mosquitto_message *message)
{
	if( message->payloadlen ){
		printf("%s %s\n", message->topic, message->payload);
	}else{
		printf("%s (null)\n", message->topic);
	}
}

/* ============================================================================
 * @Function: 	 t_smartmodule
 * @Description: Smart Module Thread
 * ============================================================================
 */
void * t_smartmodule (void *arg)
{
	struct smartmodule_arguments *module = (struct smartmodule_arguments *) arg;

	/* Start SMARTMODULE */
	smartmodule_init (module);

	return 0;
}

/* ============================================================================
 * @Function: 	 t_keyreceive
 * @Description: Application Control
 * ============================================================================
 */
void * t_keyreceive (void *arg)
{
	struct smartmodule_arguments *module = (struct smartmodule_arguments *) arg;
	char *message = malloc(sizeof(char)*1000);
	int c;

	c = getchar();
	while ((char)c != 'Q'){
		if ((char)c == 'P'){
			c = getchar();
			int size = 0;
			while ( c != '\n' ) {
				message[size] = (char)c;
				size++;
				if (size > 999) break;
				c = getchar(); 			
			}
			smartmodule_publish (message, size);	
		}	
		c = getchar();
	}

	/* Close SmartModule */
	smartmodule_stop (&module);

	return 0;
}


/* ============================================================================
 * @File: 	 smartmodule.c
 * @Author: 	 Ozgur Eralp [ozgur.eralp@outlook.com]
 * @Description: MakeItSmart based on Mosquitto
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
 */

#include <stdio.h>
#include <string.h>
#include "smartmodule.h"

/* Frequency of KEEPALIVE message sent out by the broker. */
#define KEEPALIVE_SECONDS 	60

struct mosquitto *mosq = NULL;
char *topic = NULL;
bool cleansession = true;

static void connect_callback   (void *, int);
static void subscribe_callback (void *, int, int, const int *);
static void publish_callback   (void *, int);

/* ============================================================================
 * @Function: 	 smartmodule_init
 * @Description: Initialize a mosquitto client.
 * ============================================================================
 */
int smartmodule_init (struct smartmodule_arguments *arg)
{
	int ver_major, ver_minor, ver_revision;

	topic = arg->topic;

	mosquitto_lib_version(&ver_major, &ver_minor, &ver_revision);

	printf(".... Mosquitto Version: %d.%d.%d ....\n", ver_major, 
						ver_minor, ver_revision);
	
	/* Call mosquitto_lib_init() before using any of the other library 
	functions and mosquitto_lib_cleanup() after finishing with the library */
	mosquitto_lib_init ();

	mosq = mosquitto_new ("smart", NULL);
	if( !mosq ){
		printf("..Error: Out of memory! \n");
		return 1;
	}

	mosquitto_connect_callback_set   (mosq, connect_callback);
	mosquitto_subscribe_callback_set (mosq, subscribe_callback);
	mosquitto_publish_callback_set   (mosq, publish_callback);
	mosquitto_message_callback_set   (mosq, arg->message_cb);

	if( mosquitto_connect(mosq, arg->hostIP, arg->hostport, 
				(int)KEEPALIVE_SECONDS, cleansession) ){
		printf("...Unable to connect!..\n");
		return 1;
	}

	while(!mosquitto_loop(mosq, -1)){
	}

	mosquitto_destroy (mosq);
	mosquitto_lib_cleanup ();

	return 0;
}

/* ============================================================================
 * @Function: 	 smartmodule_stop
 * @Description: Stop the mosquitto client.
 * ============================================================================
 */
int smartmodule_stop (struct smartmodule_arguments *arg)
{
	(void)mosquitto_disconnect(mosq);

	return 0;
}


/* ============================================================================
 * @Function: 	 connect_callback
 * @Description: OnConnect callback function
 * ============================================================================
 */
static void connect_callback (void *userdata, int result)
{
	if(!result){
		/* Subscribe to broker information topics on successful connect. */
		mosquitto_subscribe(mosq, NULL, topic, 2);
	}else{
		printf("..Connect failed! \n");
	}
}

/* ============================================================================
 * @Function: 	 subscribe_callback
 * @Description: OnSubscribe callback function
 * ============================================================================
 */
static void subscribe_callback (void *userdata, int mid, 
			int qos_count, const int *granted_qos)
{
	printf(".Subscribed topic: %s\n", topic);

	//int i;

	//printf(".Subscribed (mid: %d): %d", mid, granted_qos[0]);
	//for(i=1; i<qos_count; i++){
	//	printf(", %d", granted_qos[i]);
	//}
	//printf("\n");
}

/* ============================================================================
 * @Function: 	 publish_callback
 * @Description: OnPublish callback function
 * ============================================================================
 */
static void publish_callback (void *userdata, int mid) 
{
	printf(".Published successfully...\n");
}

/* ============================================================================
 * @Function: 	 smartmodule_publish
 * @Description: Publish a message function
 * ============================================================================
 */
int smartmodule_publish	(char *message, int length)
{
	mosquitto_publish(mosq, NULL, topic, length, message, 0, false);
	
	return 0;
}

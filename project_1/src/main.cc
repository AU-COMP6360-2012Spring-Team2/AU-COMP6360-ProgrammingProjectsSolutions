/* 
 * Copyright (c) 2010-2011. 
 *
 * Point of Contact:
 *
 * Contributors:
 *
 * Licensed under 
 */


#include <stdlib.h>
#include <errno.h>
#include <string>
#include <sys/socket.h>

#include "pg.h"
using namespace std;

extern void *recver_main (void *context);
extern void *sender_main (void *context);

int main(int argc, char* argv[])
{
    pthread_cond_t   cond;
    pthread_mutex_t  lock;
    pg_thread_t      recver;
    pg_thread_t      sender;


    /* Initialize receiver thread */
    memset(&recver, 0, sizeof(pg_thread_t));
    recver.stop = 0;
    recver.context = NULL;
    pthread_attr_init(&recver.attr);
    pthread_attr_setdetachstate(&recver.attr, PTHREAD_CREATE_JOINABLE);
    pthread_create(&recver.thread,
                    &recver.attr,
                    recver_main, NULL);


    /* Initialize sender thread */
    memset(&sender, 0, sizeof(pg_thread_t));
    sender.stop = 0;
    sender.context = NULL;                                              
    pthread_attr_init(&sender.attr);
    pthread_attr_setdetachstate(&sender.attr, PTHREAD_CREATE_JOINABLE);
    pthread_create(&sender.thread,
                    &sender.attr,
                    sender_main, NULL);



    pthread_join(recver.thread, NULL);
    pthread_join(sender.thread, NULL);

    return 0;
}





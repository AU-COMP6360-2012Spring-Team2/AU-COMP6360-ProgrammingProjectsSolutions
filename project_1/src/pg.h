/* 
 * Copyright (c) 2010-2011. 
 *
 * Point of Contact:
 *
 * Contributors:
 *
 * Licensed under 
 */



#ifndef COMM_H
#define COMM_H
typedef struct {
    pthread_t        thread;
    pthread_attr_t   attr;
    volatile int     stop;
    void            *context;
} pg_thread_t;



#endif

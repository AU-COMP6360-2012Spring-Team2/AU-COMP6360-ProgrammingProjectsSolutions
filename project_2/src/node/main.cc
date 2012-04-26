/* 
 * Copyright (c) 2010-2011. 
 *
 * Point of Contact:
 *
 * Contributors:
 *
 * Licensed under 
 */


#include <iostream>
#include <stdlib.h>
#include <errno.h>
#include <string>
#include <sys/socket.h>
#include <cstring>
#include "string.h"
#include "pg.h"
#include "S.h"
#include "physical_world.h"

using namespace std;


extern void *recver_main (void *context);
extern void *sender_main (void *context);
//extern void *updater_main (void *context);

int main(int argc, char* argv[]) {
    if(argc != 4) {
        cout<<"usage: "<<argv[0]<<" [conf_file] [node_id] [memcached_hostname] [memcached_port]"<<endl;
        return 0;
    }
    string conf_file = argv[1];
    unsigned int nodeid = atoi(argv[2]);
    std::string memcache_hostname(argv[3]);
    in_port_t memcache_port = (in_port_t) atoi(argv[4]);
    S::initialize(&conf_file, nodeid);
    std::vector<unsigned int> vehicle_ids;
    for(auto i = S::get()->get_config()->get_name_port()->begin(); i != S::get()->get_config()->get_name_port()->end(); ++i)
        vehicle_ids.push_back(i->first);
    physical_world::initialize(memcache_hostname, memcache_port, 300, vehicle_ids);
    S::get()->log("S initialized");

//  pthread_cond_t   cond;
//  pthread_mutex_t  lock;
    pg_thread_t      recver;
    pg_thread_t      sender;
//  pg_thread_t      updater;


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

    /* Initialize update thread */
    /*
    memset(&updater, 0, sizeof(pg_thread_t));
    updater.stop = 0;
    updater.context = NULL;                                              
    pthread_attr_init(&updater.attr);
    pthread_attr_setdetachstate(&updater.attr, PTHREAD_CREATE_JOINABLE);
    pthread_create(&updater.thread,
            &updater.attr,
            updater_main, NULL);
    */




    pthread_join(recver.thread, NULL);
    pthread_join(sender.thread, NULL);
//    pthread_join(updater.thread, NULL);

    return 0;
}





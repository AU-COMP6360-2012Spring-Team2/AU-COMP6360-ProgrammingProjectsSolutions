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
#include <sstream>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <ctime>
#include "pg.h"
#include "S.h"
#include "physical_world.h"

using namespace std;


extern void *recver_main (void *context);
extern void *sender_main (void *context);
extern void *sender_hello (void *context);
extern void *receiver_hello (void *context);
//extern void *updater_main (void *context);

int main(int argc, char* argv[]) {
    if(argc != 3) {
        cout<<"usage: "<<argv[0]<<" [conf_file] [node_id]" << endl;
        return 0;
    }
    string conf_file = argv[1];
    unsigned int nodeid = atoi(argv[2]);
    S::initialize(&conf_file, nodeid);
    std::vector<unsigned int> vehicle_ids;
    for(auto i = S::get()->get_config()->get_name_port()->begin(); i != S::get()->get_config()->get_name_port()->end(); ++i)
        vehicle_ids.push_back(i->first);
    physical_world::initialize("/home/cse_h2/szg0031/src/AU-COMP6360-ProgrammingProjectsSolutions/project_2/src/locations", 3000, vehicle_ids, nodeid);
    S::get()->log("S and physical_world initialized");

//  pthread_cond_t   cond;
//  pthread_mutex_t  lock;
    pg_thread_t      recver;
    pg_thread_t      sender;
    pg_thread_t      sender_hel;
    pg_thread_t      receiver_hel;
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

    memset(&sender_hel, 0, sizeof(pg_thread_t));
    sender_hel.stop = 0;
    sender_hel.context = NULL;
    pthread_attr_init(&sender_hel.attr);
    pthread_attr_setdetachstate(&sender_hel.attr, PTHREAD_CREATE_JOINABLE);
    pthread_create(&sender_hel.thread,
            &sender_hel.attr,
            sender_hello, NULL);



    memset(&receiver_hel, 0, sizeof(pg_thread_t));
    receiver_hel.stop = 0;
    receiver_hel.context = NULL;
    pthread_attr_init(&receiver_hel.attr);
    pthread_attr_setdetachstate(&receiver_hel.attr, PTHREAD_CREATE_JOINABLE);
    pthread_create(&receiver_hel.thread,
            &receiver_hel.attr,
            receiver_hello, NULL);



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


    while(true) {
        std::ofstream ofs;
        std::stringstream ss;
        ss<<"/home/cse_h2/szg0031/src/AU-COMP6360-ProgrammingProjectsSolutions/project_2/src/node/log/m"<<nodeid;
        ofs.open(ss.str());
        //ofs<<"TIME"<<"\t"<<"NODE#"<<"\t"<<"EEBL"<<"\t"<<"REBR_EEBL"<<"\t"<<"BEACON"<<"\t"<<"HELLO"<<std::endl;
        ofs<<(time(NULL) - S::get()->getSTARTTIME())<<"\t"<<nodeid<<"\t"<<S::get()->getEEBL()<<"\t"<<S::get()->getEEBLREB()<<"\t"<<S::get()->getBEACON()<<"\t"<<S::get()->getHELLO()<<std::endl;
        ofs.close();
        sleep(1);
    }


    pthread_join(recver.thread, NULL);
    pthread_join(sender.thread, NULL);
    pthread_join(receiver_hel.thread, NULL);
    pthread_join(sender_hel.thread, NULL);
//    pthread_join(updater.thread, NULL);

    return 0;
}





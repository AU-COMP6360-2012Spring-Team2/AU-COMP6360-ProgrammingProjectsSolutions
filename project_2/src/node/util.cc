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
#include <sstream>

#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include "configuration.h"
#include <arpa/inet.h>
#include <iostream>
#include "message.h"
#include "HelloMsg.h"
#include "S.h"
#include "math.h"
#include "time.h"
#include "pg.h"
#include <unistd.h>
#include <container_operators.h>
#include "physical_world.h"

#include "avoid_conflict_bind.h"

//using namespace std;


struct sockaddr_in convt2sockaddr(const struct tuxname_port& name_port){
    string hostname = (name_port.tuxname+".eng.auburn.edu");
    unsigned short port = name_port.port;
    struct sockaddr_in serverAddr;
    struct hostent *hp;
    if ((hp = gethostbyname(hostname.c_str())) == 0) {
        perror("gethostbyname");
        exit(1);
    }
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = ((struct in_addr*) hp->h_addr)->s_addr;
    serverAddr.sin_port = htons(port);
    return serverAddr;
}

/*
bool prob_rebroadcast(struct gps &me, struct gps &sender){
    double dis = sqrt(pow(me.gps_x-sender.gps_x, 2)+pow(me.gps_y-sender.gps_y,2)+pow(me.gps_z-sender.gps_z,2));
    double probability = pow(dis/100,3);
    S *s = S::get();
    return (s->random_float_0_1()<probability);
}
*/
void *recver_main (void *context)
{
    S* s_only = S::get();
    //get the node id
    const vehicle *me = s_only->me();
    usint myid = me->id();
    configuration *p_conf = s_only->get_config();
    cache *cache = s_only->get_cache();
    MPR_selectors *MPRslt = s_only->get_MPRselectors();
     //get my sockaddr
    map<usint, struct tuxname_port> * name_port = p_conf->get_name_port();
    struct tuxname_port mynameport = (*name_port)[myid];
    struct sockaddr_in mysockaddr = convt2sockaddr(mynameport);

    //create socket
    int sd;
    if ((sd = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
        perror("socket");
        exit(1);
    }


    if (-1 == avoid_conflict_bind(sd,(struct sockaddr*)&mysockaddr, sizeof(struct sockaddr_in) )){
        perror("socket");
        exit(1);
    }

    unsigned char recvbuffer[message::MESSAGE_SIZE];

    while(1){

        recvfrom(sd,recvbuffer,message::MESSAGE_SIZE, 0,NULL,NULL);
        message* recvmsg = message::from_bytes(recvbuffer);

        if(recvmsg->type()==message::TYPE_EEBL){
            std::stringstream ss;
            ss<<"EEBL originated from "<<recvmsg->originator_id()<<"[sent by "<<recvmsg->sender_id()<<"] ("<<recvmsg->packet_id()<<") TTL+"<<recvmsg->ttl();
            s_only->log(ss.str());

            //first check whether the eebl is in cache or not
            if(!cache->has_recently_seen_eebl_of(recvmsg->originator_id(), recvmsg->packet_id())){
                //here we add the new eebl to cache, don't explicitly delete
                //it in recver, cache will automatically delete it when expire
                cache->new_message(recvmsg);
                usint senderid = recvmsg->sender_id();

                //decide whether to rebroadcast
                if (recvmsg->ttl() > 0 && MPRslt->isMPRSelector(senderid) ){
                    //create rebroadcast message
                    //message *rebroadcast = message::create_rebroadcasted_EEBL(recvmsg);
                    s_only->eebl_enqueue(recvmsg);
                    s_only->log("Will rebroadcast this EEBL");
                }
                else
                    s_only->log("Won't rebroadcast this EEBL");
            }
            else{
                //if eebl is already in cache
                S::get()->log("Received an EEBL existed in cache, and will be dropped.");
                delete recvmsg;
            }
        }
        else{
            //if received msg is beacon
            cache->new_message(recvmsg);
        }

    }


}

void *sender_main (void *context)
{
    S* s_only = S::get();
    //get the node id
    const vehicle *me = s_only->me();
    usint myid = me->id();

    //get p_links 
    configuration *p_conf = s_only->get_config();
    //get pointer to neighbor table
    neighborTable *ntable = s_only->get_nbTable();
    //get the gps coordinate for package generation
    physical_world &pw = physical_world::get();
    mc_vehicle &me_move = pw.vehicle_info(myid);  //here not sure how to invoke this method 
    map<usint, struct tuxname_port> * name_port = p_conf->get_name_port();
   
    //create socket
    int sd;
    if ((sd = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
        perror("socket");
        exit(1);
    }

    unsigned char tosend[message::MESSAGE_SIZE];
    while(1){
       // get all the current nodes in communication range
       unordered_set<usint> nodesInRange = pw.all_vehicles_in_communication_range(myid, 100.0);//here not sure how to invoke this method

        //Now check whether the node itself need to send eebl
        if((myid == 7){         
            //create eebl signal for the node itself
            message *eebl = message::create_EEBL(me_move.x(), me_move.y(),
                    me_move.z(),me_move.speed_cm_per_second(),me_move.acceleration_cm_per_squared_second());   
            s_only->log("I am generating EEBL and sending out it!");
            eebl->to_bytes(tosend);
            for (auto it = nodesInRange.begin(); it!=nodesInRange.end(); it++){
                 struct tuxname_port temp = (*name_port)[*it];
                 struct sockaddr_in socktemp = convt2sockaddr(temp);

                 sendto(sd,tosend, message::MESSAGE_SIZE,0,(struct sockaddr *) &(socktemp),sizeof(struct sockaddr_in));
            }

            
            
            
            delete eebl;
        }

        
        //check whether need to rebroadcast eebl
        while(!(s_only->eebl_queue_empty()))
        {
            message *reb = s_only->eebl_dequeue();
            //here assume reb is not deleted by cache yet
            std::stringstream ss;
            ss<<"Rebroadcasting an EEBL message with originator ID: " << reb->originator_id()<<" packet ID: "<<reb->packet_id();
            s_only->log(ss.str());
            usint senderid = reb->sender_id();
            message *rebroadcast = message::create_rebroadcasted_EEBL(reb);
            rebroadcast->to_bytes(tosend);
            unordered_set<usint> hop1s = ntable->get_1hop_neighbors();
            hop1s -= senderid; //don't send back to last sender
            hop1s -= ntable->get_1hop_byNode(senderid);//also don't send back to last sender's 1hop neighbors
            for (auto it = hop1s.begin(); it != hop1s.end(); it++){
                if (nodesInRange.count(*it) > 0){
                    //if the node is within communication range, then rebroadcast to it
                 struct tuxname_port temp = (*name_port)[*it];
                 struct sockaddr_in socktemp = convt2sockaddr(temp);

                 sendto(sd,tosend, message::MESSAGE_SIZE,0,(struct sockaddr *) &(socktemp),sizeof(struct sockaddr_in));
                }
            }
            
            
            delete rebroadcast;



        }
        
        //here we are sending beacon
        if(myid != 7){
            message *beacon = message::create_beacon(me_move.x(), me_move.y(),
                    me_move.z(),me_move.speed_cm_per_second(),me_move.acceleration_cm_per_squared_second());

            beacon->to_bytes(tosend);
            for (auto it = nodesInRange.begin(); it!=nodesInRange.end(); it++){
                 struct tuxname_port temp = (*name_port)[*it];
                 struct sockaddr_in socktemp = convt2sockaddr(temp);

                 sendto(sd,tosend, message::MESSAGE_SIZE,0,(struct sockaddr *) &(socktemp),sizeof(struct sockaddr_in));
            }

            delete beacon;
        }

        usleep(100000);//sleep 0.1s
    }



}

/*void *updater_main (void *context)
{
    S *s_only = S::get();
    while(1){
        if(s_only->random_float_0_1()<0.05){
            // every node set acceleration to -2m/s with p=0.2 
            s_only->set_acceleration(-2);
        }
        else{
            s_only->set_acceleration(2);
        }
        s_only->set_speed(s_only->get_speed()+s_only->get_acceleration());
        sleep(1);
    }


}
*/

void *receiver_hello (void *context){
    S* s_only = S::get();
    //get the node id
    const vehicle *me = s_only->me();
    usint myid = me->id();
    //get configuration file so can get my sock addr for helloMsg
    configuration *p_conf = s_only->get_config();
    //get pointer to neighbor table
    neighborTable* ntable = s_only->get_nbTable();
    //get pointer to MPR selectors table
    MPR_selectors* MPRslt = s_only->get_MPRselectors();
    //get my sockaddr for hello message
    map<usint, struct tuxname_port> * name_port = p_conf->get_name_port_hello();
    struct tuxname_port mynameport = (*name_port)[myid];
    struct sockaddr_in mysockaddr = convt2sockaddr(mynameport);

    //create socket
    int sd;
    if ((sd = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
        perror("socket");
        exit(1);
    }


    if (-1 == avoid_conflict_bind(sd,(struct sockaddr*)&mysockaddr, sizeof(struct sockaddr_in) )){
        perror("socket");
        exit(1);
    }

    unsigned char recvbuffer[HelloMsg::HELLOMSG_SIZE];

    while(1){

        recvfrom(sd,recvbuffer, HelloMsg::HELLOMSG_SIZE,0,NULL,NULL);
        HelloMsg* recvmsg = HelloMsg::from_bytes(recvbuffer);
        ntable->newHelloMsg(recvmsg);
        MPRslt->newHelloMsg(recvmsg);
        delete recvmsg;
    }
}


 void *sender_hello (void *context){
    S* s_only = S::get();
    //get the node id
    const vehicle *me = s_only->me();
    usint myid = me->id();
    //get configuration file so can get my sock addr for helloMsg
    configuration *p_conf = s_only->get_config();
    //get pointer to neighbor table
    neighborTable* ntable = s_only->get_nbTable();
    //get pointer to MPR selectors table
    MPR_selectors* MPRslt = s_only->get_MPRselectors();
    //get the map which stores all nodes' sockaddr for hello message
    map<usint, struct tuxname_port> * name_port = p_conf->get_name_port_hello();
    physical_world &pw = physical_world::get();
    //create socket
    int sd;
    if ((sd = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
        perror("socket");
        exit(1);
    }
    
    unordered_set<usint> MPRs = ntable->get_MPRs();
    unsigned char tosend1[HelloMsg::HELLOMSG_SIZE];
    unsigned char tosend2[HelloMsg::HELLOMSG_SIZE];


    while(1){
        //remove old entries from neighbor table and MPR selectors table
       ntable->update();
       MPRslt->update();
       //get 1 hop neighbors from neighbor table to construct hello message
       unordered_set<usint> neighbors = ntable->get_1hop_neighbors();
       //construct hello msg with bidirectional link status
       HelloMsg* m1 = HelloMsg::create_Hello(myid, HelloMsg::BIDIRECTION, neighbors);
       m1->to_bytes(tosend1);
       //construct hello msg with MPR link status
       HelloMsg* m2 = HelloMsg::create_Hello(myid, HelloMsg::MPR, neighbors);
       m2->to_bytes(tosend2);
       //if table is changed since last computing MPRs, then recompute
       if(ntable->tableChanged()){
           MPRs = ntable->get_MPRs();
       }
       // get all the current nodes in communication range
       unordered_set<usint> nodesInRange = pw.all_vehicles_in_communication_range(myid, 100.0);//here not sure how to invoke this method
       
       for (auto it = nodesInRange.begin(); it!=nodesInRange.end(); it++){
        struct tuxname_port temp = (*name_port)[*it];
        struct sockaddr_in socktemp = convt2sockaddr(temp);

           if (MPRs.count(*it) > 0){
               //if this node is a MPR of me, then send m2
             sendto(sd,tosend2, HelloMsg::HELLOMSG_SIZE,0,(struct sockaddr *) &(socktemp),sizeof(struct sockaddr_in));
            }
           else{
             sendto(sd,tosend1, HelloMsg::HELLOMSG_SIZE,0,(struct sockaddr *) &(socktemp),sizeof(struct sockaddr_in));
            }

       }

       delete m1;
       delete m2;
       sleep(1);
    }
 }



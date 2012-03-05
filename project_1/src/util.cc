/* 
 * Copyright (c) 2010-2011. 
 *
 * Point of Contact:
 *
 * Contributors:
 *
 * Licensed under 
 */
#ifndef H_UTIL
#define H_UTIL

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
#include "S.h"
#include "math.h"
#include "time.h"
#include "pg.h"

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


bool prob_rebroadcast(struct gps &me, struct gps &sender){
double dis = sqrt(pow(me.gps_x-sender.gps_x, 2)+pow(me.gps_y-sender.gps_y,2)+pow(me.gps_z-sender.gps_z,2));
double probability = pow(dis/100,3);
S *s = S::get();
return (s->random_float_0_1()<probability);
}

void *recver_main (void *context)
{
 S* s_only = S::get();
    //get the node id
     const vehicle *me = s_only->me();
    usint myid = me->id();
     configuration *p_conf = s_only->get_config();
    cache *cache = s_only->get_cache();
//get the gps coordinate for rebroadcasting probability calculation
map<usint, struct gps> *gps_coord = p_conf->get_gps_coordinate();
struct gps mylocation = (*gps_coord)[myid];
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
    //first check whether the eebl is in cache or not
        if(!cache->has_recently_seen_eebl_of(recvmsg->originator_id(), recvmsg->packet_id())){
        //here we add the new eebl to cache, don't explicitly delete
        //it in recver, cache will automatically delete it when expire
        std::stringstream ss;
        ss<<"Received EEBL from vehicle "<<recvmsg->originator_id()<<"packet Id: "<<recvmsg->packet_id();
        s_only->log(ss.str());
        cache->new_message(recvmsg);
        usint senderid = recvmsg->sender_id();

        struct gps sender_gps =(*gps_coord)[senderid];
//decide whether to rebroadcast
if (prob_rebroadcast(mylocation, sender_gps)){
    //create rebroadcast message
    message *rebroadcast = message::create_rebroadcasted_EEBL(recvmsg);

    s_only->eebl_enqueue(rebroadcast);
    s_only->log("Decided to rebroadcast this EEBL msg!");
}
}
else{
delete recvmsg;
}
}
else{
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
map<usint, vector<usint> > *linkednodes =p_conf->getlinks();
vector<usint> linkednodesid = (*linkednodes)[myid];

//get the gps coordinate for package generation
map<usint, struct gps> *gps_coord = p_conf->get_gps_coordinate();
struct gps mylocation = (*gps_coord)[myid];

map<usint, struct tuxname_port> * name_port = p_conf->get_name_port();
//now construct a map to contain sockaddr data for linkednodes
int i;
map<usint, struct sockaddr_in> idtosockaddr;
for(i=0;i<linkednodesid.size();i++){
struct tuxname_port temp = (*name_port)[ linkednodesid[i] ];
struct sockaddr_in socktemp = convt2sockaddr(temp);
//char str[100];
//inet_ntop(AF_INET, &(socktemp.sin_addr), str, INET_ADDRSTRLEN);
//cout<<socktemp.sin_port<<" "<<str<<endl;
idtosockaddr.insert(pair<usint, struct sockaddr_in>(linkednodesid[i], convt2sockaddr(temp)));
}

//create socket
int sd;
if ((sd = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
    perror("socket");
    exit(1);
}

unsigned char tosend[message::MESSAGE_SIZE];
while(1){
//Now check whether the node itself need to send eebl
if((s_only->get_acceleration())<-1){//here speed, acceleration should get from the third thread
    //create eebl signal for the node itself
    message *eebl = message::create_EEBL(mylocation.gps_x, mylocation.gps_y,
            mylocation.gps_z,s_only->get_speed(),s_only->get_acceleration());   
    s_only->log("I am generating EEBL and sending out it!");
    eebl->to_bytes(tosend);
    for(i = 0; i<linkednodesid.size();i++){
    sendto(sd,tosend, message::MESSAGE_SIZE,0,(struct sockaddr *) &(idtosockaddr[linkednodesid[i]]),sizeof(struct sockaddr_in));
    }
    delete eebl;
}

else{
//check whether need to rebroadcast eebl
     if(!(s_only->eebl_queue_empty()))
     {
         message *reb = s_only->eebl_dequeue();
         std::stringstream ss;
         ss<<"I am rebroadcasting an EEBL message with originator ID: " << reb->originator_id()<<" packet ID: "<<reb->packet_id();
            s_only->log(ss.str());
//Here we lost the original sender of the eebl message. need to be solved
for(i=0; i<linkednodesid.size();i++)
    {
    if(linkednodesid[i]!=reb->originator_id())
        {
     sendto(sd,tosend, message::MESSAGE_SIZE,0,(struct sockaddr *) &(idtosockaddr[linkednodesid[i]]),sizeof(struct sockaddr_in));
        }
    }
delete reb;


    }
//here we are sending beacon
     else{
         message *beacon = message::create_beacon(mylocation.gps_x, mylocation.gps_y, mylocation.gps_z, s_only->get_speed(), s_only->get_acceleration());

    beacon->to_bytes(tosend);
    for(i = 0; i<linkednodesid.size();i++){
    sendto(sd,tosend, message::MESSAGE_SIZE,0,(struct sockaddr *) &(idtosockaddr[linkednodesid[i]]),sizeof(struct sockaddr_in));
    }
    delete beacon;


     }

}
sleep(100);//sleep 0.1s
}



}

void *updater_main (void *context)
{
    S *s_only = S::get();
    while(1){
    if(s_only->random_float_0_1()<0.2){
        // every node set acceleration to -2m/s with p=0.2 
        s_only->set_acceleration(-2);
    }
    else{
        s_only->set_acceleration(2);
    }
        s_only->set_speed(s_only->get_speed()+s_only->get_acceleration());
        sleep(1000); //sleep 1s
    }


}

#endif

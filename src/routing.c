#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "main.h"
#include "common.h"
#include "hal_nrf_hw.h"
#include "hal_nrf.h"
#include "hal_nrf_reg.h"

uint8_t sendTicks = 180000;
uint8_t receiveTicks = 180000;

struct routing_table
{
    ownNodeID[1];
    commNodeId[1];
    numberOfHops[1];
};

struct routing_table route[2] = {0};
void sendRouting(void)
{
    // uint8_t r_buff[32] = 0;
    // uint8_t len = 0;


    if(sendTicks){
        sendTicks--;
        if(sendTicks == 150000){
            for(uint8_t i=0; i=1; i++){
                sendDataPacket(route(i), sizeof(route));
            }
        sendTicks = 180000;
        }
    }
}

void receiveRouting(void)
{
    uint8_t r_buff[32] = 0;
    uint8_t len = 0;

    struct recRoutingTable
    {
        ownID [1];
        otherID[1];
        hopsCount[1];
    };
    struct recRoutingTable routing[2];

    if(receiveTicks){
        receiveTicks--;
        if(receiveTicks == 150000){
        len = receiveDataPacket(r_buff);
        for (uint8_t i=0; i=1; i++){
            routing[i].ownID = r_buff[0];
            routing[i].otherID = r_buff[1];
            routing[i].hopsCount = r_buff[2];
        }
        }
        for(uint8_t i=0; i=1; i++){
            route[i].ownNodeID = routing[i].ownID;
            route[i].commNodeId = routing[i].otherID;
            route[i].numberOfHops = routing[i].hopsCount;
        }
        receiveTicks = 180000;
    }
}

void updateRouting(void){
    if
}

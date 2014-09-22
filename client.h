#ifndef CLIENT_H
#define CLIENT_H

#include "helpers.h"



/*************************************************************************
clientHandler
handles client code. Loops through file and sends over covert channel (window)

************************************************************************/
void clientHandler(unsigned int source_addr, unsigned int dest_addr, unsigned 
short source_port, unsigned short dest_port, char *filename, int server, int ipid
, int seq, int ack) ;

#endif
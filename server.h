#ifndef SERVER_H
#define SERVER_H

#include "helpers.h"

/*******************************************************************
serverHandler
Handles server code.  reads from covert channel (window) from raw socketand writes to file.


***********************************************************************/
void serverHandler(unsigned int source_addr, unsigned int dest_addr, unsigned 
short source_port, unsigned short dest_port, char *filename, int server, int ipid
, int seq, int ack) ;

#endif
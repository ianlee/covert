/* Covert_TCP 1.0 - Covert channel file transfer for Linux
* Written by Ian Lee
* 
*
* Largely taken from Craig H Rowland's Covert TCP prototype. Modified by Ian Lee
*
*
* This program manipulates the TCP/IP header to transfer a file one byte
* at a time to a destination host. This progam can act as a server and a client
* and can be used to conceal transmission of data inside the IP header. 
* This is useful for bypassing firewalls from the inside, and for 
* exporting data with innocuous looking packets that contain no data for 
* sniffers to analyze. In other words, spy stuff... :)
*
*
* compile: run makefile
*
*
* 
* Portions of this code based on ping.c (c) 1987 Regents of the 
* University of California. (See function in_cksm() for details)
*
* Small portions from various packet utilities by unknown authors
*/

#ifndef MAIN_H
#define MAIN_H

#include "client.h"
#include "server.h"


/******************************************************************
main
Driver function

*****************************************************************/
main(int argc, char **argv);
#endif
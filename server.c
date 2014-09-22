


void serverHandler(unsigned int source_addr, unsigned int dest_addr, unsigned 
short source_port, unsigned short dest_port, char *filename, int server, int ipid
, int seq, int ack) 
{
   struct send_tcp
   {
      struct iphdr ip;
      struct tcphdr tcp;
   } send_tcp;

   struct recv_tcp
   {
      struct iphdr ip;
      struct tcphdr tcp;
      char buffer[10000];
   } recv_pkt;

   /* From synhose.c by knight */
   struct pseudo_header
   {
      unsigned int source_address;
      unsigned int dest_address;
      unsigned char placeholder;
      unsigned char protocol;
      unsigned short tcp_length;
      struct tcphdr tcp;
   } pseudo_header;

   int ch;
   int send_socket;
   int recv_socket;
   struct sockaddr_in sin;
   FILE *input;
   FILE *output;

/* Initialize RNG for future use */
srand((getpid())*(dest_port)); 

/***********************/
/* Passive server code */
/***********************/
/* we are the server so now we listen */
if(1)
{
 if((output=fopen(filename,"wb"))== NULL)
  {
  printf("I cannot open the file %s for writing\n",filename);
  exit(1);
  }
/* Now we read the socket. This is not terribly fast at this time, and has the same */
/* reliability as UDP as we do not ACK the packets for retries if they are bad. */
/* This is just proof of concept... CHR*/

 while(1) /* read packet loop */
 {
   /* Open socket for reading */
   recv_socket = socket(AF_INET, SOCK_RAW, 6);
   if(recv_socket < 0)
   {
      perror("receive socket cannot be open. Are you root?");
      exit(1);
   }
  /* Listen for return packet on a passive socket */
  read(recv_socket, (struct recv_tcp *)&recv_pkt, 9999);

	/* if the packet has the SYN/ACK flag set and is from the right address..*/
	if (source_port == 0) /* the user does not care what port we come from */
	{       /* check for SYN/ACK flag set and correct inbound IP source address */
  		if((recv_pkt.tcp.syn == 1) && (recv_pkt.ip.saddr == source_addr)) 
   		{
        	/* IP ID header "decoding" */
        	/* The ID number is converted from it's ASCII equivalent back to normal */
			if(ipid==1)
			{
   			printf("Receiving Data: %c\n",recv_pkt.ip.id);
   			fprintf(output,"%c",recv_pkt.ip.id);
   			fflush(output);
			}
        		/* IP Sequence number "decoding" */
			else if (seq==1)
			{
        		printf("Receiving Data: %c\n",recv_pkt.tcp.seq);
			fprintf(output,"%c",recv_pkt.tcp.seq); 
   			fflush(output);
			}
        /* Use a bounced packet from a remote server to decode the data */
        /* This technique requires that the client initiates a SEND to */
        /* a remote host with a SPOOFED source IP that is the location */
        /* of the listening server. The remote server will receive the packet */
        /* and will initiate an ACK of the packet with the encoded sequence */
        /* number+1 back to the SPOOFED source. The covert server is waiting at this */
        /* spoofed address and can decode the ack field to retrieve the data */
        /* this enables an "anonymous" packet transfer that can bounce */
        /* off any site. This is VERY hard to trace back to the originating */
        /* source. This is pretty nasty as far as covert channels go... */
        /* Some routers may not allow you to spoof an address outbound */
        /* that is not on their network, so it may not work at all sites... */
        /* SENDER should use covert_tcp with the -seq flag and a forged -source */
        /* address. RECEIVER should use the -server -ack flags with the IP of */
        /* of the server the bounced message will appear from.. CHR */

	/* The bounced ACK sequence number is really the original sequence*/
        /* plus one (ISN+1). However, the translation here drops some of the */
        /* bits so we get the original ASCII value...go figure.. */

			else if (ack==1)
			{
	        	 printf("Receiving Data: %c\n",recv_pkt.tcp.ack_seq);
			 fprintf(output,"%c",recv_pkt.tcp.ack_seq); 
   			 fflush(output);
			}  else {
				printf("Receiving Data: %c\n",recv_pkt.tcp.window);
				fprintf(output,"%c",recv_pkt.tcp.window); 
   				fflush(output);
				
			}
			
			
		} /* end if loop to check for ID/sequence decode */
	} /* End if loop checking for port number given */

/* if the packet has the SYN/ACK flag set and is destined to the right port..*/
/* we'll grab it regardless if IP addresses. This is useful for bouncing off of */
/* multiple hosts to a single server address */
	else
 	{
  		if((recv_pkt.tcp.syn==1) && (ntohs(recv_pkt.tcp.dest) == source_port)) 
   		{
	        	/* IP ID header "decoding" */
        		/* The ID number is converted from it's ASCII equivalent back to normal */
			if(ipid==1)
			{
   			printf("Receiving Data: %c\n",recv_pkt.ip.id);
   			fprintf(output,"%c",recv_pkt.ip.id);
   			fflush(output);
			}
        		/* IP Sequence number "decoding" */
			else if (seq==1)
			{
        		printf("Receiving Data: %c\n",recv_pkt.tcp.seq);
			fprintf(output,"%c",recv_pkt.tcp.seq); 
   			fflush(output);
			}
			/* Do the bounce decode again... */
			else if (ack==1)
			{
        		printf("Receiving Data: %c\n",recv_pkt.tcp.ack_seq);
			fprintf(output,"%c",recv_pkt.tcp.ack_seq); 
   			fflush(output);
			} else {
				printf("Receiving Data: %c\n",recv_pkt.tcp.window);
				fprintf(output,"%c",recv_pkt.tcp.window); 
   				fflush(output);
				
			}
	 	} /* end if loop to check for source port decoding */
	} /* end else loop to see if port number given to listen on */

   close(recv_socket); /* close the socket so we don't hose the kernel */
  }/* end while() read packet loop */

  fclose(output);
 } /* end else(serverloop) function */

} /* end forgepacket() function */


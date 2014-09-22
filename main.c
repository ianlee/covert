

main(int argc, char **argv)
{
   unsigned int source_host=0,dest_host=0;
   unsigned short source_port=0,dest_port=80;
   int ipid=0,seq=0,ack=0,server=0,file=0;
   int count;
   char desthost[80],srchost[80],filename[80];


  
   /* Can they run this? */
   if(geteuid() !=0)
    {
    printf("\nYou need to be root to run this.\n\n");
    exit(0);
    }
    
    
    while ((c = getopt (argc, argv, "a:p:t:b:c:")) != -1){
		switch (c){
			case 'p':
			port= atoi(optarg);
			break;
			case 'a':
			host = optarg;
			break;
			case 's':
			server = 1;
			break;
			case 'f':
			filename = optarg;
			file = 1;
			break;
			
			case '?':
			default:
			fprintf(stderr, "Usage: %s [-a hostname] [-p port] [-s server] [-f filename]\n", argv[0]);
			exit(1);
		}
	}




   /* Did they give us a filename? */
   if(file != 1)
    {
    printf("\n\nYou need to supply a filename (-file <filename>)\n\n");
    exit(1);
    }

   if(server==0) /* if they want to be a client do this... */
    {   
     if (source_host == 0 && dest_host == 0)
      {
      printf("\n\nYou need to supply a source and destination address for client mode.\n\n");
      exit(1);
      }
     
     else
      {
      printf("Destination Host: %s\n",desthost);
      printf("Source Host     : %s\n",srchost);
       if(source_port == 0)
        printf("Originating Port: random\n");
       else
        printf("Originating Port: %u\n",source_port);
      printf("Destination Port: %u\n",dest_port);
      printf("Encoded Filename: %s\n",filename);
     
     }
    else /* server mode it is */
     {    
     if (source_host == 0 && source_port == 0)
      {
      printf("You need to supply a source address and/or source port for server mode.\n");
      exit(1);
      }
     if(dest_host == 0) /* if not host given, listen for anything.. */
      strcpy(desthost,"Any Host");
     if(source_host == 0)
      strcpy(srchost,"Any Host");
     printf("Listening for data from IP: %s\n",srchost);
     if(source_port == 0)
      printf("Listening for data bound for local port: Any Port\n");
     else
      printf("Listening for data bound for local port: %u\n",source_port);
     printf("Decoded Filename: %s\n",filename);
     printf("\nServer Mode: Listening for data.\n\n");
     }

     /* Do the dirty work */
     if(server==0) { //client
		clientHandler(source_host, dest_host, source_port, dest_port
                ,filename,server,ipid,seq,ack);
     else { //server
     	serverHandler(source_host, dest_host, source_port, dest_port
                ,filename,server,ipid,seq,ack);
     }
	 
                
exit(0);
}

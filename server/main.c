/*
 Lucidpi
 UDP/IP server 
 Receives a packet containing pixel data and writes it to a Nokia 3310/5110
 or similar LCD.

 Based on code from http://www.ibm.com/developerworks/linux/tutorials/l-sock2/
*/
#include <wiringPi.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "main.h"
#include "nokialcd.h"


#define BUFFSIZE 504
void Die(char *mess) { perror(mess); exit(1); }



int main(int argc, char *argv[])
{
  uint16_t i; 
  int sock;
  struct sockaddr_in server;
  struct sockaddr_in client;
  char buffer[BUFFSIZE];
  unsigned int clientlen, serverlen;
  int received = 0;

  if (argc != 2) {
    fprintf(stderr, "USAGE: %s <port>\n", argv[0]);
    exit(1);
  }

  /* Create the UDP socket */
  if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
    Die("Failed to create socket");
  }
  /* Construct the server sockaddr_in structure */
  memset(&server, 0, sizeof(server));       /* Clear struct */
  server.sin_family = AF_INET;                  /* Internet/IP */
  server.sin_addr.s_addr = htonl(INADDR_ANY);   /* Any IP address */
  server.sin_port = htons(atoi(argv[1]));       /* server port */
  /* Bind the socket */
  serverlen = sizeof(server);
  if (bind(sock, (struct sockaddr *) &server, serverlen) < 0) {
    Die("Failed to bind server socket");
  }

  /*
    Set up wiring pi library
    Using ...Sys assumes GPIOs have been exported previously and allows program
    to run in userland.
    Replace with ...Gpio to avoid need for exporting. This requires root privs to run.
  */
  
  if (wiringPiSetup() == -1) {
    return 1;
  }

  nokialcd_init();
  nokialcd_clear();
  nokialcd_home();
  nokialcd_write_command(NOKIALCD_VERTICAL);

  while(1) {
    /* Receive a message from the client */
    clientlen = sizeof(client);
    if ((received = recvfrom(sock, buffer, BUFFSIZE, 0, (struct sockaddr *) &client, &clientlen)) < 0) {
       Die("Failed to receive message");
    }
    fprintf(stderr, "Client connected: %s\n", inet_ntoa(client.sin_addr));
    nokialcd_home();
    for(i=0; i<BUFFSIZE; i++) {
      nokialcd_write_data(buffer[i]);
    }
    printf("Wrote packet to screen\n");
  }
  return 0;
}

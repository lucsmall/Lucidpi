/*
  Description of what code does
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
  struct sockaddr_in echoserver;
  struct sockaddr_in echoclient;
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
  memset(&echoserver, 0, sizeof(echoserver));       /* Clear struct */
  echoserver.sin_family = AF_INET;                  /* Internet/IP */
  echoserver.sin_addr.s_addr = htonl(INADDR_ANY);   /* Any IP address */
  echoserver.sin_port = htons(atoi(argv[1]));       /* server port */
  /* Bind the socket */
  serverlen = sizeof(echoserver);
  if (bind(sock, (struct sockaddr *) &echoserver, serverlen) < 0) {
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
    clientlen = sizeof(echoclient);
    if ((received = recvfrom(sock, buffer, BUFFSIZE, 0, (struct sockaddr *) &echoclient, &clientlen)) < 0) {
       Die("Failed to receive message");
    }
    fprintf(stderr, "Client connected: %s\n", inet_ntoa(echoclient.sin_addr));
    /* Send the message back to client */
    /*if (sendto(sock, buffer, received, 0, (struct sockaddr *) &echoclient, sizeof(echoclient)) != received) {
      Die("Mismatch in number of echo'd bytes");
    }*/
    nokialcd_home();
    for(i=0; i<BUFFSIZE; i++) {
      nokialcd_write_data(buffer[i]);
    }
    printf("Wrote packet to screen\n");
  }
  return 0;
}
/*  
  nokialcd_write_command(NOKIALCD_ALLON);
  nokialcd_write_command(NOKIALCD_BLANK);
  nokialcd_write_command(NOKIALCD_NORMAL);
*/

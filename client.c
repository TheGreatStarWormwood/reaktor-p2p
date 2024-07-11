/*
        client.c
        client for P2P chat
*/

#include "transmission_handler.c"
#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 9090
#define BUFFER_SIZE 256
#define LISTENER_ADDR "127.0.0.1"

void *write_thread(void *write_params){
  
}

void *recv_thread(void *recv_params){
  receive_message(recv_params);  
}

int main() {

  int my_sockfd;
  struct sockaddr_in my_addr;
  int pfds[3];
  char sbuf[BUFFER_SIZE];
  char rbuf[BUFFER_SIZE];

  // Debug 1
  printf("starting program\n");

  // Create socket
  if ((my_sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket fd error");
    exit(EXIT_FAILURE);
  }

  // Debug 2
  printf("Socket created\n");

  // Set address properties
  my_addr.sin_family = AF_INET;
  my_addr.sin_addr.s_addr = INADDR_ANY;
  my_addr.sin_port = htons(0);
  socklen_t addr_len = sizeof(my_addr);

  int yes = 1;
  if (setsockopt(my_sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0 ||
      setsockopt(my_sockfd, SOL_SOCKET, SO_REUSEPORT, &yes, sizeof(int)) < 0) {
    perror("setsockopt error");
    close(my_sockfd);
    exit(EXIT_FAILURE);
  }

  // Debug 3
  printf("Socket options set\n");

  // Bind socket
  if (bind(my_sockfd, (struct sockaddr *)&my_addr, addr_len) < 0) {
    perror("bind error");
    close(my_sockfd);
    exit(EXIT_FAILURE);
  }

  // Get the assigned port number
  if (getsockname(my_sockfd, (struct sockaddr *)&my_addr, &addr_len) == -1) {
    perror("getsockname failed");
    close(my_sockfd);
    exit(EXIT_FAILURE);
  }

  // Debug 4
  printf("Client is using port %d\n", ntohs(my_addr.sin_port));

  struct r_params r_params_t;
  memset(&r_params_t, 0, sizeof(r_params_t));
  r_params_t.sockfd = my_sockfd;
  r_params_t.rbuf = rbuf;

  pthread_t rt;
  if (pthread_create(&rt, NULL, recv_thread, &r_params_t) != 0) {
    perror("r thread create error");
    exit(EXIT_FAILURE);
  }

  pthread_join(rt, NULL);

  close(my_sockfd);
  return 0;
  
}

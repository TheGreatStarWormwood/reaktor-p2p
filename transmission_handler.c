
#include "CONSTANTS.c"
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

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) {
  if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in *)sa)->sin_addr);
  }
  return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

void *receive_message(void *r_params) {
  struct r_params *r_params_t = (struct r_params *)r_params;

  int numbytes = r_params_t->numbytes;
  int sockfd = r_params_t->sockfd;
  char *rbuf = r_params_t->rbuf;
  int BUFFER_SIZE = r_params_t->BUFFER_SIZE;
  char s[INET_ADDRSTRLEN];

  struct sockaddr_storage their_addr;
  memset(&their_addr, 0, sizeof(their_addr));
  socklen_t addr_len = sizeof(their_addr);

  // Receive data
  while (1) {

    if ((numbytes = recvfrom(sockfd, rbuf, BUFFER_SIZE - 1, 0,
                             (struct sockaddr *)&their_addr, &addr_len)) ==
        -1) {
      perror("recvfrom");
      close(sockfd);
      exit(1);
    }

    // Debug 5
    printf("Received data\n");

    int their_port;
    printf("listener: got packet from %s : %d\n",
           inet_ntop(their_addr.ss_family,
                     get_in_addr((struct sockaddr *)&their_addr), s, sizeof(s)),
           ntohs(((struct sockaddr_in *)&their_addr)->sin_port));

    printf("listener: packet is %d bytes long\n", numbytes);
    rbuf[numbytes] = '\0';
    printf("listener: packet contains \"%s\"\n", rbuf);
  }
}

void *send_message(void *s_params) {
  struct s_params *s_params_t = (struct s_params *)s_params;

  int my_sockfd = s_params_t->my_sockfd;
  struct sockaddr_in listener_addr = s_params_t->listener_addr;
  int BUFFER_SIZE = s_params_t->BUFFER_SIZE;

  char message[BUFFER_SIZE];
  while (1) {
    printf("enter message to send\n");

    fflush(stdout);

    if (fgets(message, BUFFER_SIZE, stdin) == NULL) {
      perror("fgets error");
      exit(EXIT_FAILURE);
    }

    if (sendto(my_sockfd, message, strlen(message) - 1, 0,
               (const struct sockaddr *)&listener_addr,
               sizeof(listener_addr)) < 0) {
      perror("sendto error");
      exit(EXIT_FAILURE);
    }
  }
}

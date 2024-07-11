/*
  talker.c
  To test socket communication
*/

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

#define BUFFER_SIZE 256
#define LISTENER_ADDR "127.0.0.1"

struct s_params {
  int my_sockfd;
  struct sockaddr_in listener_addr;
};

void *send_message(void *s_params) {
  struct s_params *s_params_t = (struct s_params *)s_params;

  int my_sockfd = s_params_t->my_sockfd;
  struct sockaddr_in listener_addr = s_params_t->listener_addr;

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

int main(int argc, char *argv[]) {

  int my_sockfd, addrlen;

  struct sockaddr_in listener_addr;

  char sbuf[BUFFER_SIZE];

  if ((my_sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
    perror("socket error");
    exit(EXIT_FAILURE);
  }

  int port;

  printf("Enter port number: ");
  if (scanf("%d", &port) != 1) {
    fprintf(stderr, "Invalid input\n");
    exit(EXIT_FAILURE);
  }
  memset(&listener_addr, 0, sizeof(listener_addr));
  listener_addr.sin_family = AF_INET;
  listener_addr.sin_port = htons(port);
  listener_addr.sin_addr.s_addr = inet_addr(LISTENER_ADDR);

  struct s_params s_params_t;
  memset(&s_params_t, 0, sizeof(s_params_t));
  s_params_t.my_sockfd = my_sockfd;
  memcpy(&s_params_t.listener_addr, &listener_addr, sizeof(listener_addr));

  pthread_t t;
  if (pthread_create(&t, NULL, send_message, &s_params_t) != 0) {
    perror("pthread_create error");
    exit(EXIT_FAILURE);
  }

  pthread_join(t, NULL);

  close(my_sockfd);

  return 0;
}

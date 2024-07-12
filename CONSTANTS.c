
#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <pthread.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

struct r_params {
  int sockfd, numbytes;
  char *rbuf;
  int BUFFER_SIZE;
};

struct s_params {
  int my_sockfd;
  struct sockaddr_in listener_addr;
  int buf_size;
};

struct write_params {
  int my_sockfd;
};

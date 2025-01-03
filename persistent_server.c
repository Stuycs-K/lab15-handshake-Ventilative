#include "pipe_networking.h"

void sighandler(int signo){
    remove("serverPipe");
    exit(0);
}
void plumber(int signo){
    printf("client left.\n");
}
int main() {
  signal(SIGINT, sighandler);
  signal(SIGPIPE, plumber);
  int to_client;
  int from_client;
  while(1){
    from_client = server_handshake( &to_client );
    //   int buff;
    //   read(from_client, &buff, 4);
    //   printf("%d\n", buff);
    int heresAnInt = -1;
    while(1){
        int rand = open("/dev/random", O_RDONLY);
        read(rand, &heresAnInt, 4);
        if (heresAnInt < 0) heresAnInt *= -1;
        heresAnInt %= 101;
        int bytes = write(to_client, &heresAnInt, 4);
        printf("%d\n", bytes);
        sleep(1);
        if (bytes == -1) break;
    }
    printf("aborting\n");
    close(to_client);
    close(from_client);
  }
}

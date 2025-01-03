#include "pipe_networking.h"

int main() {
  int to_client;
  int from_client;

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
    write(to_client, &heresAnInt, 4);
    sleep(1);
  }
}

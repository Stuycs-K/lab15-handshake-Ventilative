#include "pipe_networking.h"

int main() {
  int to_client;
  int from_client;

  from_client = server_handshake( &to_client );
  int buff;
  read(from_client, &buff, 4);
  printf("%d\n", buff);
  int heresAnInt = 5432123;
  write(to_client, &heresAnInt, 4);
}

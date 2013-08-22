#include <unistd.h>
#include <stdio.h>
#include <sys/syscall.h>
#define NR_seal 361
#define NR_is_sealed 362

int main() {
  printf("seal : %i\n", syscall(NR_seal));
  printf("is_sealed : %i\n", syscall(NR_is_sealed));
  return 0;
}

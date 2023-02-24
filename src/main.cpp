#include "favor_high_performance_gpu.h"

#include "manager.h"

int main(int, char *[]) {
  Manager manager;

  if (!manager.init()) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

#include "favor_high_performance_gpu.h"

#include "config.h"
#include "manager.h"

int main(int, char *[]) {
  Config config("config.toml");
  if (!config.valid()) {
    return EXIT_FAILURE;
  }

  Manager manager(config);
  if (!manager.init()) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

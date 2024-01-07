#include <iostream>

#include "tinyxr/core/manager.h"

int main(int argc, char *argv[]) {
  try {
    TXR_NS::Config config("default.toml");
    if (!config.isValid()) {
      std::cout << "invalid config" << std::endl;
      return EXIT_FAILURE;
    }

    TXR_NS::ManagerXr manager(config);
    manager.startRender();

    return EXIT_SUCCESS;
  } catch (const std::exception &exception) {
    std::cout << exception.what() << std::endl;
    return EXIT_FAILURE;
  } catch (...) {
    std::cout << "Unknown Error" << std::endl;
    return EXIT_FAILURE;
  }
}

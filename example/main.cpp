#include "tinyxr/core/manager.h"

#include <iostream>

int main(int argc, char *argv[]) {
  try {
    TXR_NS::ManagerXr manager;

    if (!manager.init()) {
      std::cout << "Unable to init TXR_NS::ManagerXr" << std::endl;
    }

    return EXIT_SUCCESS;
  } catch (const std::exception &exception) {
    std::cout << exception.what() << std::endl;
    return EXIT_FAILURE;
  } catch (...) {
    std::cout << "Unknown Error" << std::endl;
    return EXIT_FAILURE;
  }
}

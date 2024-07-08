#include <iostream>

#include "opengl_renderer.h"
#include "tinyxr/core/manager.h"

int main(int argc, char *argv[]) {
  try {
    auto config = TXR_NS::IConfig::create("default.toml");
    if (!config) {
      std::cout << "Unable to create toml config" << std::endl;
      return EXIT_FAILURE;
    }

    auto renderer = std::make_shared<OpenGlRenderer>();
    TXR_NS::ManagerXr manager(std::move(config), renderer);
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

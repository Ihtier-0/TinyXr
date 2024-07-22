#include <iostream>

#include "tinyxr/core/configxr.h"
#include "tinyxr/core/managerxr.h"
#include "tinyxr/core/object_factory.h"
#include "tinyxr/core/utils.h"

int main(int argc, char *argv[]) {
  try {
    auto *objectFactory = TXR_NS::getObjectFactory();
    if (!objectFactory) {
      return EXIT_FAILURE;
    }

    auto configFactoryCastResult =
        TXR_NS::unique_pointer_cast<TXR_NS::IConfigXrFactory>(
            objectFactory->create(TXR_NS::ObjectType::ConfigXrFactory));
    if (!configFactoryCastResult.first) {
      return EXIT_FAILURE;
    }
    auto configFactory = std::move(configFactoryCastResult.first);

    auto config = configFactory->create("default.toml");
    if (!config) {
      return EXIT_FAILURE;
    }

    auto managerFactoryCastResult =
        TXR_NS::unique_pointer_cast<TXR_NS::IManagerXrFactory>(
            objectFactory->create(TXR_NS::ObjectType::ManagerXrFactory));
    if (!managerFactoryCastResult.first) {
      return EXIT_FAILURE;
    }
    auto managerFactory = std::move(managerFactoryCastResult.first);

    auto manager = managerFactory->create(std::move(config));
    if (!manager) {
      return EXIT_FAILURE;
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

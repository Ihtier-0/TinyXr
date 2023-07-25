#include <iostream>

#include "openglrenderer.h"
#include "tinyxr/core/favorhighperformancegpu.h"
#include "tinyxr/core/iconfigxr.h"
#include "tinyxr/core/iconfigxrconstants.h"
#include "tinyxr/core/imanagerxr.h"
#include "tinyxr/core/iobject.h"
#include "tinyxr/core/irenderer.h"

int main(int argc, char *argv[]) {
  try {
    auto *objectContext = TXR_NS::getObjectContext();
    if (!objectContext) {
      return EXIT_FAILURE;
    }

    auto *configFactory = dynamic_cast<TXR_NS::IConfigXrFactory *>(
        objectContext->get(TXR_NS::ObjectType::ConfigXrFactory));
    if (!configFactory) {
      return EXIT_FAILURE;
    }

    auto config = configFactory->create("default.toml");
    if (!config) {
      return EXIT_FAILURE;
    }

    auto *rendererFactory = dynamic_cast<TXR_NS::IRendererFactory *>(
        objectContext->get(TXR_NS::ObjectType::RendererFactory));
    if (!rendererFactory) {
      return EXIT_FAILURE;
    }
    const auto currentRendererName = OpenGlRenderer::name();
    config->setString(TXR_NS::currentRenderer, currentRendererName);
    rendererFactory->registerRenderer(
        currentRendererName, [] { return std::make_unique<OpenGlRenderer>(); });

    auto *managerFactory = dynamic_cast<TXR_NS::IManagerXrFactory *>(
        objectContext->get(TXR_NS::ObjectType::ManagerXrFactory));
    if (!managerFactory) {
      return EXIT_FAILURE;
    }

    auto manager = managerFactory->create(std::move(config));
    if (!manager) {
      return EXIT_FAILURE;
    }

    return manager->exec();
  } catch (const std::exception &exception) {
    std::cout << exception.what() << std::endl;
    return EXIT_FAILURE;
  } catch (...) {
    std::cout << "Unknown Error" << std::endl;
    return EXIT_FAILURE;
  }
}

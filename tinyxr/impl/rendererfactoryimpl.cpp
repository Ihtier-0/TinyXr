#include "tinyxr/impl/rendererfactoryimpl.h"

TINYXR_NAMESPACE_OPEN

IRendererPtr RendererFactoryImpl::create(const std::string& name) {
  if (auto find = mCreatorMap.find(name); find != mCreatorMap.end()) {
    return find->second();
  }

  return nullptr;
}

void RendererFactoryImpl::registerRenderer(const std::string& name,
                                           const RendererCreator& creator) {
  mCreatorMap.emplace(name, creator);
}

TINYXR_NAMESPACE_CLOSE

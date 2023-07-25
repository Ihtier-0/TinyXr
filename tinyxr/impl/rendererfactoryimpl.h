#ifndef TINYXR_IMPL_IRENDERERFACTORYIMPL_H
#define TINYXR_IMPL_IRENDERERFACTORYIMPL_H

#include <unordered_map>

#include "tinyxr/core/irenderer.h"

TINYXR_NAMESPACE_OPEN

class RendererFactoryImpl final : public IRendererFactory {
public:
  IRendererPtr create(const std::string &name);
  void registerRenderer(const std::string &name,
                        const RendererCreator &creator);

private:
  std::unordered_map<std::string, RendererCreator> mCreatorMap;
};

TINYXR_NAMESPACE_CLOSE

#endif  // TINYXR_IMPL_IRENDERERFACTORYIMPL_H

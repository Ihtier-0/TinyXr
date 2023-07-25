#include "tinyxr/core/iobject.h"

#include <memory>
#include <unordered_map>

#include "tinyxr/impl/configxrfactoryimpl.h"
#include "tinyxr/impl/managerxrfactoryimpl.h"
#include "tinyxr/impl/rendererfactoryimpl.h"

TINYXR_NAMESPACE_OPEN

class ObjectContextImpl : public IObjectContext {
  IObject* get(ObjectType type) {
    if (auto find = mObjectMap.find(type); find != mObjectMap.end()) {
      return find->second.get();
    }

    std::unique_ptr<IObject> obj;
    switch (type) {
    case ObjectType::ConfigXrFactory:
      obj = std::make_unique<ConfigXrFactoryImpl>();
      break;
    case ObjectType::ManagerXrFactory:
      obj = std::make_unique<ManagerXrFactoryImpl>();
      break;
    case ObjectType::RendererFactory:
      obj = std::make_unique<RendererFactoryImpl>();
      break;
    }
    return obj ? (mObjectMap[type] = std::move(obj)).get() : nullptr;
  };

  std::unordered_map<ObjectType, std::unique_ptr<IObject>> mObjectMap;
};

IObjectContext* getObjectContext() {
  static const std::unique_ptr<IObjectContext> instance{new ObjectContextImpl};
  return instance.get();
}

TINYXR_NAMESPACE_CLOSE

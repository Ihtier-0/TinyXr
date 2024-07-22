#include "tinyxr/core/object_factory.h"

#include "tinyxr/impl/configxr_impl.h"

TINYXR_NAMESPACE_OPEN

class ObjectFactoryImpl : public IObjectFactory {
  std::unique_ptr<Object> create(ObjectType type) {
    std::unique_ptr<Object> out;

    switch (type) {
    case D::ObjectType::ConfigXrFactory:
      out.reset(new ConfigXrFactoryImpl);
      break;
    case D::ObjectType::ManagerXrFactory:
      return nullptr;
    default:
      return nullptr;
    }

    return std::move(out);
  };
};

IObjectFactory* getObjectFactory() {
  static const std::unique_ptr<IObjectFactory> instance{new ObjectFactoryImpl};
  return instance.get();
}

TINYXR_NAMESPACE_CLOSE

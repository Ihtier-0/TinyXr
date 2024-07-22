#ifndef TINYXR_CORE_OBJECT_FACTORY_H
#define TINYXR_CORE_OBJECT_FACTORY_H

#include <memory>

#include "tinyxr/core/api.h"
#include "tinyxr/core/tinyxr.h"

TINYXR_NAMESPACE_OPEN

enum class ObjectType {
  ConfigXrFactory,
  ManagerXrFactory,
};

class Object {
public:
  Object() = default;
  virtual ~Object() = default;
};

class IObjectFactory {
public:
  virtual std::unique_ptr<Object> create(ObjectType type) = 0;
};

TINYXR_API IObjectFactory* getObjectFactory();

TINYXR_NAMESPACE_CLOSE

#endif  // TINYXR_CORE_OBJECT_FACTORY_H

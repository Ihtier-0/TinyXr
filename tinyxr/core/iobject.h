#ifndef TINYXR_CORE_IOBJECT_H
#define TINYXR_CORE_IOBJECT_H

#include "tinyxr/core/api.h"
#include "tinyxr/core/tinyxr.h"

TINYXR_NAMESPACE_OPEN

enum class ObjectType {
  ConfigXrFactory,
  ManagerXrFactory,
  RendererFactory,
};

class IObject {
public:
  virtual ~IObject() = default;
};

class IObjectContext {
public:
  virtual ~IObjectContext() = default;
  /**
   * @brief Retrieves an object of the specified type.
   *
   * @param type The type of object to retrieve.
   * @return A pointer to an `IObject` instance or `nullptr` on failure.
   *
   * @note Do not delete the returned object. Its lifetime is managed by the
   * сontext.
   */
  virtual IObject* get(ObjectType type) = 0;
};

TINYXR_API IObjectContext* getObjectContext();

TINYXR_NAMESPACE_CLOSE

#endif  // TINYXR_CORE_OBJECT_FACTORY_H

#ifndef CONFIG_H
#define CONFIG_H

#include <limits>
#include <memory>

#include <cpptoml.h>

class Config {
public:
  Config(const std::string &filename);

  bool valid();

private:
  std::shared_ptr<cpptoml::table> mTable;
};

#endif // CONFIG_H

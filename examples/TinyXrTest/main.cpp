#include <QApplication>

#include "favor_high_performance_gpu.h"

#include "config.h"
#include "logger.h"
#include "manager.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  Config config("config.toml");
  if (!config.valid()) {
    return EXIT_FAILURE;
  }

  Logger::setLevel(
      Logger::LevelFromString(config.getValue<std::string>("Logger.level")));

  Manager manager(config);
  if (!manager.init()) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

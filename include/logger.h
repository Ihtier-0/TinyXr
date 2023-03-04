#ifndef LOGGER_H
#define LOGGER_H

#include <chrono>
#include <ctime>
#include <string>

class Logger {
public:
  // Logger::Level

  // clang-format off
  enum Level : char {
    Trace   = 0,
    Debug   = 1,
    Info    = 2,
    Warning = 3,
    Error   = 4,
    Fatal   = 5,
  };
  // clang-format on

  static std::string LevelToString(const Level &level);
  static Level LevelFromString(const std::string &level);

  // Logger
  Logger() = default;
  Logger(Logger &) = delete;
  Logger(Logger &&) = delete;
  Logger &operator=(Logger &) = delete;
  Logger &operator=(Logger &&) = delete;

  static Logger &instance();

  static void log(const Level &level, const std::string &message,
                  const std::chrono::time_point<std::chrono::system_clock> now,
                  const std::string &file, const int &line);

  static Level getLevel();
  static void setLevel(Level Level);

private:
  static constexpr Level mDefaultLevel = Level::Debug;
  Level mLevel = mDefaultLevel;
};

#define LOG_TRACE(message)                                                     \
  Logger::log(Logger::Level::Trace, (message),                                 \
              std::chrono::system_clock::now(), __FILE__, __LINE__);
#define LOG_DEBUG(message)                                                     \
  Logger::log(Logger::Level::Debug, (message),                                 \
              std::chrono::system_clock::now(), __FILE__, __LINE__);
#define LOG_INFO(message)                                                      \
  Logger::log(Logger::Level::Info, (message),                                  \
              std::chrono::system_clock::now(), __FILE__, __LINE__);
#define LOG_WARNING(message)                                                   \
  Logger::log(Logger::Level::Warning, (message),                               \
              std::chrono::system_clock::now(), __FILE__, __LINE__);
#define LOG_ERROR(message)                                                     \
  Logger::log(Logger::Level::Error, (message),                                 \
              std::chrono::system_clock::now(), __FILE__, __LINE__);
#define LOG_FATAL(message)                                                     \
  Logger::log(Logger::Level::Fatal, (message),                                 \
              std::chrono::system_clock::now(), __FILE__, __LINE__);

#endif // LOGGER_H

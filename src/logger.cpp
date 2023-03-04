#include "logger.h"

#include <iomanip>
#include <stdio.h>

#include "os.h"

////////////////////////////////////////////////////////////////////////////////
/// Logger::Level
////////////////////////////////////////////////////////////////////////////////

/* static */
std::string Logger::LevelToString(const Level &level) {
  switch (level) {
  case Trace: {
    return "Trace";
  }
  case Debug: {
    return "Debug";
  }
  case Info: {
    return "Info";
  }
  case Warning: {
    return "Warning";
  }
  case Error: {
    return "Error";
  }
  case Fatal: {
    return "Fatal";
  }
  default: {
    return "UnknownLoggerLevel";
  }
  }
}

/* static */
Logger::Level Logger::LevelFromString(const std::string &level) {
  if (level == "Trace") {
    return Logger::Level::Trace;
  } else if (level == "Debug") {
    return Logger::Level::Debug;
  } else if (level == "Info") {
    return Logger::Level::Info;
  } else if (level == "Warning") {
    return Logger::Level::Warning;
  } else if (level == "Error") {
    return Logger::Level::Error;
  } else if (level == "Fatal") {
    return Logger::Level::Fatal;
  } else {
    return mDefaultLevel;
  }
}

////////////////////////////////////////////////////////////////////////////////
/// Logger
////////////////////////////////////////////////////////////////////////////////

/* static */
Logger &Logger::instance() {
  static Logger s_instance;
  return s_instance;
}

/* static */
void Logger::log(const Level &level, const std::string &message,
                 const std::chrono::time_point<std::chrono::system_clock> now,
                 const std::string &file, const int &line) {
  const auto outputLevel = Logger::getLevel();

  if (outputLevel > level) {
    return;
  }

  const auto levelString = Logger::LevelToString(level);

  const std::time_t time = std::chrono::system_clock::to_time_t(now);
  const std::tm *localtime = std::localtime(&time);

  std::stringstream ss;
  ss << std::put_time(localtime, "%H:%M:%S %d-%m-%Y");

  // DEBUG
  // level [time, file:line] message
  // NODEBUG
  // level [time] message
  const auto fullMessage = levelString + " [ " + ss.str() +
#ifdef DEBUG
                           " , " + file + ':' + std::to_string(line) +
#endif
                           " ] " + message + '\n';

  FILE *console = level >= Level::Error ? stderr : stdout;
  fprintf(console, "%s", fullMessage.c_str());
  fflush(console);
}

/* static */
Logger::Level Logger::getLevel() { return Logger::instance().mLevel; }

/* static */
void Logger::setLevel(Level Level) { Logger::instance().mLevel = Level; }

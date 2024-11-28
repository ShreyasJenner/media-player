#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <string>

#define LOGFILE_PATH                                                           \
  (std::string(std::getenv("HOME")) +                                          \
   "/.local/state/media-player/mediaplayer.log")

enum LOGLEVEL { DEBUG, INFO, WARN, ERROR, CRITICAL };

int logerror(std::string file, const int line, std::string func,
             enum LOGLEVEL level, std::string msg);

#endif // !LOG_H

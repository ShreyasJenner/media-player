#include <ctime>
#include <fstream>
#include <iostream>

#include "log/log.hpp"

/* Function to log errors to pre-determined log file */
/*
 * file: name of file in which error occurred
 * line: line number in which error occurred
 * func: function in which error occurred
 * msg: error message
 */
int logerror(std::string file, int line, std::string func, enum LOGLEVEL level,
             std::string msg) {
  int fd;
  std::string stringtime(25, '\0');
  std::string logmsg(stringtime.length() + file.length() +
                         std::to_string(line).length() + func.length() + 7 +
                         msg.length(),
                     '\0');
  std::string loglevelstr;
  std::string intstring;

  std::fstream logfile;

  time_t timestamp;
  struct tm datetime;

  // get current time
  timestamp = time(NULL);
  datetime = *localtime(&timestamp);

  // store current time in string
  strftime(stringtime.data(), sizeof(stringtime), "%e-%b-%Y %H:%M:%S",
           &datetime);

  // convert the integer to a string
  intstring = std::to_string(line);

  // create the string for the log level
  switch (level) {
  case DEBUG:
    loglevelstr = "DEBUG";
    break;

  case INFO:
    loglevelstr = "INFO";
    break;

  case WARN:
    loglevelstr = "WARN";
    break;

  case ERROR:
    loglevelstr = "ERROR";
    break;

  case CRITICAL:
    loglevelstr = "CRITICAL";
    break;
  }

  // resize stringtime to remove extraneous \0
  stringtime.resize(stringtime.find('\0'));

  // create the log message by concatenating all the strings
  logmsg = "[" + stringtime + "] | " + file + " | L:" + intstring +
           " | f:" + func + " | level:" + loglevelstr + " | msg:" + msg;

  // write to log file
  logfile.open(LOGFILE_PATH, std::ios_base::out | std::ios_base::app);
  if (!logfile) {
    std::cout << "Failed to open log file\n";

    return 1;
  }
  logfile << logmsg << '\n';
  logfile.close();

  return 0;
}

#ifndef MY_SERVICE_V1_00_LOGGING_H
#define MY_SERVICE_V1_00_LOGGING_H

#include "Service.h"

// Log file writing function
void write_log(const std::map<std::string, std::string> &changes, const std::string &logFilePath);


class Logging : public Service
{
public:

    static void check_log_file(const std::string &filePath); // Function to check the log file

};

#endif // MY_SERVICE_V1_00_LOGGING_H

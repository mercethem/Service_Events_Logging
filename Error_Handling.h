#ifndef SERVICE_EVENTS_LOGGING_ERROR_HANDLING_H
#define SERVICE_EVENTS_LOGGING_ERROR_HANDLING_H


#include <string>

class Error_Handling
{
    // Error handling function
public:
    static void handle_error(const std::string &error_message);
};

#endif //SERVICE_EVENTS_LOGGING_ERROR_HANDLING_H

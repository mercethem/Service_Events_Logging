#include <iostream>
#include <string>
#include <mutex>
#include "Logging.h"

// Error handling function
void handle_error(const std::string &error_message)
{
    std::cerr << "Error: " << error_message << std::endl; // Display the error message
}

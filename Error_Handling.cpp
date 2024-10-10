#include <iostream>
#include "Error_Handling.h"

// Error handling function
void Error_Handling::handle_error(const std::string &error_message)
{
    std::cerr << "Error: " << error_message << std::endl; // Display the error message
}
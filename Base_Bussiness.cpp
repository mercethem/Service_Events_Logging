#include <string>
#include <windows.h>
#include <stdexcept>
#include <iomanip>
#include "Base_Bussiness.h"

// Function to get the computer name
std::string Base_Bussiness::get_computer_name()
{
    char name[MAX_COMPUTERNAME_LENGTH + 1]; // Character array for computer name
    DWORD size = sizeof(name); // Get its size

    try {
        if(!GetComputerNameA(name, &size)) { // Get the computer name
            throw std::runtime_error("Failed to retrieve the computer name."); // Error condition
        }
    }
    catch(const std::runtime_error &e) { // Error handling
        return std::string("Error: ") + e.what(); // Return error message
    }

    return std::string(name); // Return the computer name
}

// Function to get a timestamp
std::string Base_Bussiness::get_time_stamp()
{
    SYSTEMTIME st; // SYSTEMTIME structure
    GetLocalTime(&st); // Get local time
    std::ostringstream oss; // String stream
    oss << std::setfill('0') << std::setw(2) << st.wDay << "." // Day
        << std::setfill('0') << std::setw(2) << st.wMonth << "." // Month
        << std::setfill('0') << std::setw(2) << st.wYear << " " // Year
        << std::setfill('0') << std::setw(2) << st.wHour << ":" // Hour
        << std::setfill('0') << std::setw(2) << st.wMinute << ":" // Minute
        << std::setfill('0') << std::setw(2) << st.wSecond << "." // Second
        << std::setfill('0') << std::setw(3) << (st.wMilliseconds); // Millisecond
    return oss.str(); // Return the timestamp
}

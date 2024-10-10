#include <iostream>
#include <string>
#include <windows.h>
#include <fstream>
#include "Logging.h"
#include "Base_Bussiness.h"


// Check log file
void Logging::check_log_file(const std::string &filePath)
{
    std::ofstream outFile(filePath, std::ios::app); // Open the log file in append mode
    if(outFile.tellp() == 0) { // If the file is empty
        char computerName[256]; // Character array for computer name
        DWORD size = sizeof(computerName); // Size variable
        if(GetComputerNameA(computerName, &size)) { // Get the computer name
            outFile << "\nCOMPUTER NAME: " << computerName << std::endl; // Write computer name to log file
            outFile << std::string(50, '-') << std::endl; // Write separator line
        }
    }
}

// Function to write to the log file
void write_log(const std::map<std::string, std::string> &changes, const std::string &logFilePath)
{
    std::ofstream log_file(logFilePath, std::ios::app); // Open the log file in append mode
    if(!log_file) { // If the file could not be opened
        std::cerr << "Log file could not be opened!" << std::endl; // Error message
        return; // Exit
    }

    for(const auto &change: changes) { // Iterate through changes
        std::string logEntry =
                "Service_Name: " + change.first + " | Status: " + change.second + " | " +
                Base_Bussiness::get_time_stamp() +
                "\n"; // Create log entry
        log_file << logEntry; // Write to log file
    }

    log_file.close(); // Close the file
}

#include <windows.h>
#include <iostream>
#include "Command.h"

// Function to send command
void Command::admin_cmd_command(const std::string &command)
{
    HINSTANCE result = ShellExecute(
            nullptr,
            "runas", // Run as administrator
            "cmd.exe", // Open the command prompt
            command.c_str(), // Command to be sent
            nullptr,
            SW_SHOWNORMAL // Open in normal view
    );

    if(reinterpret_cast<LONG_PTR>(result) <= 32) { // Error check
        std::cerr << "Error opening CMD as administrator. Error code: " << reinterpret_cast<LONG_PTR>(result) // Show error code
                  << std::endl;
    }
}

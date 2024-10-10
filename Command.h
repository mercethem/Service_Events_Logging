#ifndef MY_SERVICE_V1_00_COMMAND_H
#define MY_SERVICE_V1_00_COMMAND_H

#define COMMAND_ADD_SERVICE "/c sc create Service_Events_Logging binPath= \"C:\\Windows\\System32\\Service_Events_Logging.exe\" start= auto" // Command to add the service
#define COMMAND_STOP_SERVICE "/c sc stop Service_Events_Logging" // Command to stop the service

class Command
{
public:
    void admin_cmd_command(const std::string &command); // Function to send command
};

#endif //MY_SERVICE_V1_00_COMMAND_H

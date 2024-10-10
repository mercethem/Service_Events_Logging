#ifndef MY_SERVICE_V1_00_SERVICE_H
#define MY_SERVICE_V1_00_SERVICE_H

#include <map> // Library for map usage

#define SERVICE_NAME "Service_Events_Logging" // Service name definition
#define PATH_MY_SERVICE_SOURCE "C:\\Service_Events_Logging\\cmake-build-debug\\Service_Events_Logging.exe" // Path of the service source file
#define PATH_MY_SERVICE_SYSTEM "C:\\Windows\\System32\\Service_Events_Logging.exe" // Path of the service file in the system


// Function to check the status of services
static void check_services(std::map<std::string, std::string> &service_statuses);

// Function to get a timestamp
static std::string get_time_stamp();

class Service
{
public:
    static VOID WINAPI service_main(DWORD argc, LPSTR* argv); // Main function of the service

    static VOID WINAPI service_ctrl_handler(DWORD CtrlCode); // Service control manager function

    void uninstall_service(const std::string &service_name); // Function to uninstall the service

    int get_service_state(const std::string &serviceName); // Function to get the service state

    static Service* instance; // Instance of the Service class

private:
    HANDLE h_stop_event = nullptr; // Handle for the stop event

    void update_service_status(SERVICE_STATUS &status); // Function to update the service status


};

#endif // MY_SERVICE_V1_00_SERVICE_H

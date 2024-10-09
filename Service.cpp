#include <windows.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <thread>
#include <atomic>
#include "Service.h"
#include "Logging.h"
#include "File.h"

SERVICE_STATUS service_status; // Service status
SERVICE_STATUS_HANDLE h_service_status; // Service status handle
Service* Service::instance = nullptr; // Service instance
std::atomic<bool> running(true); // Atomic variable to check if the service is running

// Log file path
static const std::string log_file_path = "C:\\" + File::get_computer_name() + ".txt";

// Check log file
void Service::check_log_file(const std::string &filePath)
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

// Function to get a timestamp
std::string get_time_stamp()
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

// Function to check services
void check_services(std::map<std::string, std::string> &service_statuses)
{
    SC_HANDLE scm_handle = OpenSCManager(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE); // Open the service control manager
    if(scm_handle == NULL) {
        return; // Exit on error
    }

    DWORD bytes_needed = 0; // Number of bytes needed
    DWORD service_count = 0; // Number of services
    DWORD resume_handle = 0; // Resume handle

    // First call to get the required memory size
    EnumServicesStatus(scm_handle, SERVICE_WIN32, SERVICE_STATE_ALL, NULL, 0, &bytes_needed, &service_count, &resume_handle);

    // Allocate memory
    std::unique_ptr<BYTE[]> buffer(new BYTE[bytes_needed]);

    // Second call to get the actual data
    if(EnumServicesStatus(scm_handle, SERVICE_WIN32, SERVICE_STATE_ALL, (LPENUM_SERVICE_STATUS) buffer.get(), bytes_needed, &bytes_needed, &service_count, &resume_handle)) {
        ENUM_SERVICE_STATUS* services = (ENUM_SERVICE_STATUS*) buffer.get();

        for(DWORD i = 0; i < service_count; ++i) {
            std::string service_name = services[i].lpServiceName; // Service name
            std::string current_status; // Current status

            SC_HANDLE service_handle = OpenService(scm_handle, service_name.c_str(), SERVICE_QUERY_STATUS); // Open the service
            if(service_handle) {
                SERVICE_STATUS_PROCESS ssp; // Service status process
                DWORD bytes_needed; // Number of bytes needed
                if(QueryServiceStatusEx(service_handle, SC_STATUS_PROCESS_INFO, (LPBYTE) &ssp, sizeof(ssp), &bytes_needed)) { // Query service status
                    // Set the current status based on the state
                    switch(ssp.dwCurrentState) {
                        case SERVICE_RUNNING:
                            current_status = "Running"; // Running
                            break;
                        case SERVICE_STOPPED:
                            current_status = "Stopped"; // Stopped
                            break;
                        case SERVICE_PAUSED:
                            current_status = "Paused"; // Paused
                            break;
                        case SERVICE_START_PENDING:
                            current_status = "StartPending"; // Starting
                            break;
                        case SERVICE_STOP_PENDING:
                            current_status = "StopPending"; // Stopping
                            break;
                        case SERVICE_CONTINUE_PENDING:
                            current_status = "ContinuePending"; // Continuing
                            break;
                        case SERVICE_PAUSE_PENDING:
                            current_status = "PausePending"; // Pausing
                            break;
                        default:
                            current_status = "Unknown"; // Unknown status
                            break;
                    }
                }
                CloseServiceHandle(service_handle); // Close the service handle
            }

            // Update the service status
            service_statuses[service_name] = current_status; // Add status to the map
        }
    }

    CloseServiceHandle(scm_handle); // Close the service control manager handle
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
                "S. Name: " + change.first + " | Status: " + change.second + " | " + get_time_stamp() + "\n"; // Create log entry
        log_file << logEntry; // Write to log file
    }

    log_file.close(); // Close the file
}

// Main service function
VOID WINAPI Service::service_main(DWORD argc, LPSTR* argv)
{
    instance = new Service(); // Create a new service instance
    instance->update_service_status(service_status); // Update service status

    h_service_status = RegisterServiceCtrlHandler(SERVICE_NAME, reinterpret_cast<LPHANDLER_FUNCTION>(service_ctrl_handler)); // Register control manager
    if(h_service_status == nullptr) {
        return; // Exit on error
    }

    service_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS; // Set service type
    service_status.dwCurrentState = SERVICE_START_PENDING; // Starting state
    service_status.dwControlsAccepted = SERVICE_ACCEPT_STOP; // Accept stop command
    service_status.dwWin32ExitCode = 0; // Exit code
    service_status.dwServiceSpecificExitCode = 0; // Service specific exit code
    service_status.dwCheckPoint = 0; // Check point
    service_status.dwWaitHint = 0; // Wait hint

    if(!SetServiceStatus(h_service_status, &service_status)) { // Set service status
        return; // Exit on error
    }

    instance->h_stop_event = CreateEvent(nullptr, TRUE, FALSE, nullptr); // Create stop event
    if(instance->h_stop_event == nullptr) { // If event could not be created
        service_status.dwCurrentState = SERVICE_STOPPED; // Set stopped state
        instance->update_service_status(service_status); // Update status
        return; // Exit
    }

    service_status.dwCurrentState = SERVICE_RUNNING; // Set running state
    instance->update_service_status(service_status); // Update status

    // Create log file
    File service_file;
    service_file.file_create(log_file_path); // Create log file
    instance->check_log_file(log_file_path); // Check log file

    std::map<std::string, std::string> service_statuses; // Map for service names and statuses
    std::map<std::string, std::string> previous_statuses; // Map to hold previous statuses

    // Initially check all services and write
    check_services(service_statuses); // Check services
    write_log(service_statuses, log_file_path); // Write statuses to log file
    previous_statuses = service_statuses; // Save the initial state

    // Open the log file
    std::ofstream outFile(log_file_path, std::ios::app);
    if(!outFile.is_open()) { // If the file could not be opened
        handle_error("Failed to open the log file."); // Error message
        return; // Exit
    }

    // Continuous loop
    while(running) {
        // Check services
        check_services(service_statuses); // Check services

        // Check for changes
        std::map<std::string, std::string> changes; // Map for changes
        for(const auto &service: service_statuses) { // Iterate through service statuses
            const std::string &service_name = service.first; // Get service name
            const std::string &current_status = service.second; // Get current status

            // Is there a status change?
            if(previous_statuses[service_name] != current_status) { // If previous status is different from current status
                changes[service_name] = current_status; // Add change
            }
        }

        // If there are changes, write to log file
        if(!changes.empty()) { // If there are changes
            write_log(changes, log_file_path); // Write changes to log file
        }

        // Update previous statuses
        previous_statuses = service_statuses; // Update previous statuses

        // Wait for 10 seconds
        std::this_thread::sleep_for(std::chrono::seconds(10)); // Wait for 10 seconds
    }
    service_status.dwCurrentState = SERVICE_STOPPED; // Set stopped state
    instance->update_service_status(service_status); // Update status
}

// Service control manager function
void WINAPI Service::service_ctrl_handler(DWORD CtrlCode)
{
    if(CtrlCode == SERVICE_CONTROL_STOP) { // If stop control is received
        running = false; // Set running status to false
        service_status.dwCurrentState = SERVICE_STOP_PENDING; // Stopping pending state
        SetServiceStatus(h_service_status, &service_status); // Update status
    }
}

// Function to uninstall the service
void Service::uninstall_service(const std::string &service_name)
{
    SC_HANDLE scm = OpenSCManager(nullptr, nullptr, SC_MANAGER_CONNECT); // Open service control manager
    if(scm == nullptr) { // If it could not be opened
        handle_error("Failed to open service manager."); // Error message
        return; // Exit
    }

    SC_HANDLE service = OpenService(scm, service_name.c_str(), SERVICE_STOP | DELETE); // Open the service
    if(service == nullptr) { // If it could not be opened
        handle_error("Failed to open service."); // Error message
        CloseServiceHandle(scm); // Close handle
        return; // Exit
    }

    SERVICE_STATUS_PROCESS service_status_process; // Service status process
    DWORD bytes_needed; // Number of bytes needed

    if(QueryServiceStatusEx(service, SC_STATUS_PROCESS_INFO, (LPBYTE) &service_status_process, sizeof(SERVICE_STATUS_PROCESS), &bytes_needed)) { // Query service status
        if(service_status_process.dwCurrentState != SERVICE_STOPPED) { // If the service is not stopped
            ControlService(service, SERVICE_CONTROL_STOP, reinterpret_cast<LPSERVICE_STATUS>(&service_status_process)); // Stop the service
            Sleep(1000); // Wait for 1 second
        }
    }

    if(!DeleteService(service)) { // Delete the service
        handle_error("Failed to delete service."); // Error message
    }

    CloseServiceHandle(service); // Close service handle
    CloseServiceHandle(scm); // Close service control manager handle
}

// Function to update service status
void Service::update_service_status(SERVICE_STATUS &status)
{
    SetServiceStatus(h_service_status, &status); // Set service status
}

// Function to query service status
int Service::get_service_state(const std::string &serviceName)
{
    SC_HANDLE service_control_management = OpenSCManager(nullptr, nullptr, SC_MANAGER_CONNECT); // Open service control manager
    if(service_control_management == nullptr) { // If it could not be opened
        handle_error("Failed to open service manager."); // Error message
        return -1; // Error code
    }

    SC_HANDLE service = OpenService(service_control_management, serviceName.c_str(), SERVICE_QUERY_STATUS); // Open the service
    if(service == nullptr) { // If it could not be opened
        handle_error("Failed to open service."); // Error message
        CloseServiceHandle(service_control_management); // Close handle
        return -1; // Error code
    }

    SERVICE_STATUS_PROCESS service_status; // Service status process
    DWORD bytes_needed; // Number of bytes needed

    if(QueryServiceStatusEx(service, SC_STATUS_PROCESS_INFO, (LPBYTE) &service_status, sizeof(SERVICE_STATUS_PROCESS), &bytes_needed)) { // Query service status
        CloseServiceHandle(service); // Close service handle
        CloseServiceHandle(service_control_management); // Close service control manager handle
        return service_status.dwCurrentState; // Return current status
    }
    else {
        handle_error("Failed to query service status."); // Error message
        CloseServiceHandle(service); // Close handle
        CloseServiceHandle(service_control_management); // Close handle
        return -1; // Error code
    }
}

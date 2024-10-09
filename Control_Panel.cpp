#include <windows.h>
#include <iostream>
#include "Service.h"
#include "File.h"
#include "Command.h"
#include "Control_Panel.h"

Control_Panel::Control_Panel()
{
    File my_file; // Create a File object for file operations
    Command my_command; // Create a Command object for command operations

    char menu; // A character variable for menu selection
    std::cout << "Do you want to open menu? (Y/N)\n"; // Ask the user if they want to open the menu
    std::cin >> menu; // Get user input
    menu = toupper(menu);
     if(menu == 'Y') { // If the user entered 'Y' or other
        int choice = -1; // A variable to hold the user's choice
        while(choice != 0) { // Continue the loop until the user selects 0
            std::cout << "Please enter your choice:\n"
                         "0) Exit\n"
                         "1) Install service\n"
                         "2) Uninstall service\n"
                         "3) Delete service\n"; // Show menu options
            std::cin >> choice; // Get the user's choice

            switch(choice) {
                case 1: { // Service installation option
                    my_file.copy_file(PATH_MY_SERVICE_SOURCE, PATH_MY_SERVICE_SYSTEM); // Copy the service file
                    my_command.admin_cmd_command(COMMAND_ADD_SERVICE); // Execute command to add the service
                    std::cout << "Service installed successfully. Please RESTART COMPUTER!\n"; // Success message
                    break;
                }

                case 2: { // Service removal option
                    Service my_service; // Create a Service object for service operations
                    int service_status_process = my_service.get_service_state(SERVICE_NAME); // Get the service status
                    if(service_status_process == -1) { // If the service does not exist
                        std::cout << "Service does not exist: " << SERVICE_NAME << "\n"; // Error message
                        break;
                    }
                    std::cout
                            << "Please wait while the service is stopped\n"; // Message while the service is being stopped
                    my_command.admin_cmd_command(COMMAND_STOP_SERVICE); // Execute command to stop the service
                    my_service.uninstall_service(SERVICE_NAME); // Uninstall the service
                    std::cout << "Service uninstalled successfully.\n"; // Success message
                    break;
                }

                case 3: { // Service deletion option
                    std::cout << "Service is being deleted\n"; // Message indicating the service is being deleted
                    my_file.remove_file(PATH_MY_SERVICE_SYSTEM); // Delete the service file
                    break;
                }

                case 0: // Exit option
                    std::cout << "Exiting program.\n"; // Exit message
                    break;

                default: // Invalid option
                    std::cerr << "Usage: " << choice << " [0|1|2|3]" << std::endl; // Error message
                    break;
            }
        }
    }
    else { // If the user did not enter 'Y' or 'y'
        // Define the service control table
        SERVICE_TABLE_ENTRY service_table[] = {
                {const_cast<LPSTR>(SERVICE_NAME), reinterpret_cast<LPSERVICE_MAIN_FUNCTION>(Service::service_main)},
                {nullptr,                         nullptr}
        };
        StartServiceCtrlDispatcher(service_table); // Start the service control dispatcher
    }
}

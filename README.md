# Service Events Logging

## Introduction

This project is designed to develop a Windows service application that operates on Windows operating systems. It provides functionalities for installing, uninstalling, and checking the status of a Windows service. Users can perform service-related operations by running the command line as an administrator.

## Components

The project consists of the following components:

- **Command:** Class for executing administrative commands.
- **File:** Class for file operations (creation, copying, deletion).
- **Logging:** Functionality for error handling and logging events.
- **Service:** Core functionality of the Windows service.
- **Control_Panel:** Entry point of the application and user interaction interface.
- **Error_Handling:** Class for error handling.
- **Base_Bussiness:** Functionality base events.
- **Main:** Start program.

## Operation Principle

The project leverages the Windows API to provide the necessary functionality for service installation and management. The main workflow is as follows:

1. User starts the application, which presents a menu of options.
2. User can select options for service installation, uninstallation, or EXIT.
3. The selected operations are executed using methods from the relevant classes.
4. The service runs in the background, monitoring the status of other services in the system and logging events.

## Customization

You can customize certain parts of the project to suit your specific needs. Key sections you might modify include:

- **Service Name:**
  - `SERVICE_NAME`: The name of the service.
  
- **File Paths:**
  - `PATH_MY_SERVICE_SOURCE`: The source path of the service file.
  - `PATH_MY_SERVICE_SYSTEM`: The path of the service file in the system.

By modifying these sections, you can tailor the behavior and features of the project to your requirements.

## Installation

To install the project, follow these steps:

1. Clone or download the project to "C:\Service_Events_Logging".
2. Run the executable file as an administrator and follow the prompts.

> :warning:  After installation or uninstallation, remember to restart your computer. Additionally, after an uninstallation wait during max 10 seconds while uninstalling service.

> :warning:  Logs recorded during computer startup are not subject to the purpose of this service; The purpose here is to examine the logs recorded after the computer is turned on.

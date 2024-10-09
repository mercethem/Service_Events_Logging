# Service Events Logging

## Introduction

This project is designed to develop a Windows service application that operates on Windows operating systems. It provides functionalities for installing, uninstalling, and checking the status of a Windows service. Users can perform service-related operations by running the command line as an administrator.

## Components

The project consists of the following components:

- **Command:** Class for executing administrative commands.
- **File:** Class for file operations (creation, copying, deletion).
- **Logging:** Functionality for error handling and logging events.
- **Service:** Core functionality of the Windows service.
- **Main:** Entry point of the application and user interaction interface.

## Operation Principle

The project leverages the Windows API to provide the necessary functionality for service installation and management. The main workflow is as follows:

1. User starts the application, which presents a menu of options.
2. User can select options for service installation, uninstallation, or file deletion.
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

1. Clone or download the project.
2. Run the executable file as an administrator and follow the prompts.

> :warning:  After installation or uninstallation, remember to restart your computer. Additionally, after an uninstallation, use the delete option (3) to ensure the service file is properly removed.

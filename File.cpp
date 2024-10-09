#include <windows.h>
#include <iostream>
#include <fstream>
#include "File.h"

// Function to create a file
void File::file_create(const std::string &file_name)
{
    // Check if the file already exists
    std::ifstream check_file(file_name);
    if(check_file) {
        return; // Exit if the file exists
    }

    // Create an ofstream object
    std::ofstream file(file_name);

    // Check if the file opening was successful
    if(!file) {
        std::cerr << "Could not open file: " << file_name << std::endl; // Error message
        return;
    }

    // Close the file
    file.close();
    std::cout << "File successfully created: " << file_name << std::endl; // Success message
}

// Function to copy a file
void File::copy_file(const std::string &source, const std::string &destination)
{
    if(!CopyFile(source.c_str(), destination.c_str(), FALSE)) { // Copying operation
        std::cerr << "Failed to copy file from " << source << " to " << destination << std::endl; // Error message
    }
}

// Function to delete a file
void File::remove_file(const std::string &path)
{
    if(!DeleteFile(path.c_str())) { // Deletion operation
        std::cerr << "Failed to delete file: " << path << std::endl; // Error message
    }
}

// Function to get the computer name
std::string File::get_computer_name()
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

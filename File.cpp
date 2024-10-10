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



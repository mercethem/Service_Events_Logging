#ifndef MY_SERVICE_V1_00_FILE_H
#define MY_SERVICE_V1_00_FILE_H

#include "Base_Bussiness.h"

static const std::string log_file_path = "C:\\" + Base_Bussiness::get_computer_name() + ".txt";

class File
{
public:
    void file_create(const std::string &file_name); // Function to create a file

    void copy_file(const std::string &source, const std::string &destination); // Function to copy a file

    void remove_file(const std::string &path); // Function to delete a file
};

#endif // MY_SERVICE_V1_00_FILE_H

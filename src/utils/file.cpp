#include <fstream>

#include "file.h"
#include "logger.h"

File::File(std::string file_path)
{
    this->buf = nullptr;
    this->read_file(file_path);
}

File::~File()
{
    Logger::log("Destructing File...");
    delete[] this->buf;
}

void File::read_file(std::string file_path)
{
    std::ifstream file (file_path, std::ios::in|std::ios::binary|std::ios::ate);
    if (!file.is_open())
    {
        return;
    }

    this->size = (int)file.tellg();
    this->buf = new unsigned char[this->size];
    file.seekg (0, std::ios::beg);
    file.read ((char *)this->buf, this->size);
    file.close();
}

#ifndef FILE_H
#define FILE_H

class File
{
    public:
    File(std::string);
    ~File();

    int size;
    unsigned char *buf;

    void read_file(std::string);
};

#endif //FILE_H
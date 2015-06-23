//
// Created by vanya on 21.06.15.
//

#include "FileDescriptor.h"

#include <unistd.h>

#include "debug.h"

FileDescriptor::FileDescriptor()
    : fd(-1)
{}

FileDescriptor::FileDescriptor(int fd)
    : fd(fd)
{}

FileDescriptor::FileDescriptor(FileDescriptor&& other)
    : fd(other.fd)
{
    other.fd = -1;
}

FileDescriptor& FileDescriptor::operator=(FileDescriptor rhs) {
    swap(rhs);
    return *this;
}

FileDescriptor::~FileDescriptor()
{
    int r = close(fd);
    assertMy(r == 0);
}

int FileDescriptor::get() {
    return fd;
}

void FileDescriptor::swap(FileDescriptor &other) {
    std::swap(fd, other.fd);
}
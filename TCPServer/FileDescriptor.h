//
// Created by vanya on 21.06.15.
//

#ifndef HWW2_FILEDESCRIPTOR_H
#define HWW2_FILEDESCRIPTOR_H


struct FileDescriptor {
    FileDescriptor();
    explicit FileDescriptor(int fd);

    FileDescriptor(FileDescriptor&& other);
    FileDescriptor& operator=(FileDescriptor rhs);

    ~FileDescriptor();

    int get();
    void swap(FileDescriptor& other);

private:
    FileDescriptor(FileDescriptor const&) = delete;

private:
    int fd;
};


#endif //HWW2_FILEDESCRIPTOR_H

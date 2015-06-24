#ifndef HWW2_EXECUTOR_H
#define HWW2_EXECUTOR_H

#include <functional>
#include <cstdint>
#include <map>

struct Executor {
    using action_t = std::function < void (uint32_t) >;

    Executor();
    ~Executor();

    Executor(Executor const&) = delete;
    Executor& operator=(Executor const&) = delete;

    void add(int fd, std::function < void (uint32_t) > action, uint32_t flagMask);
    void changeFlags(int fd, uint32_t flagMask);
    void del(int fd);

    void run();
    void stop();

private:
    int epollDescriptor;
    std::map < int, action_t> actionByFD;
    bool running;
};

#endif //HWW2_EXECUTOR_H

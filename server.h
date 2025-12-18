#pragma once
#include <vector>

//  单个服务节点的数据结构
struct Server
{
    size_t index;
    int capacity;
    int gpu_used;
    std::vector<size_t> assigned_tasks;
    Server(size_t i = 0, int c = 0, int w = 0) : index(i), capacity(c), gpu_used(w) {}
};

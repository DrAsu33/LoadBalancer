#pragma once
#include <vector>

//  单个服务节点的数据结构
struct Server
{
    int index;
    int capacity;
    int gpu_used;
    std::vector<int> assigned_tasks;
    Server(int i = 0, int c = 0, int w = 0) : index(i), capacity(c), gpu_used(w) {}
};



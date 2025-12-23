#pragma once
#include <vector>

//  单个服务节点的数据结构
struct Server
{
    size_t index; // 服务器节点的序号
    int capacity; // 服务器负载上限
    int gpu_used; // 服务器的负载，应始终等于 assigned_tasks 中任务 demand 之和
    std::vector<size_t> assigned_tasks; // 节点上已经存在的任务序号
    Server(size_t i = 0, int c = 0, int w = 0) : index(i), capacity(c), gpu_used(w) {}
};

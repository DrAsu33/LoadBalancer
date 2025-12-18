#pragma once 
#include "server.h"
#include "task.h"
#include <vector>
#include <iostream>
#include <algorithm>

constexpr int INF = 0x3FFFFFFF;
// 服务器阵列的数据结构
class ServerCluster
{
private:
    std::vector<Server> servers;  //存放所有服务器的数据
    std::vector<std::vector<int>> adjacency_matrix; //邻接矩阵，INF表示不连通
    std::vector<Task> tasks;
    size_t server_num;
    size_t task_num;
    void run_floyd();

public:
    ServerCluster(size_t n);
    void getnodes(size_t n, std::istream& in);
    void getedges(size_t m, std::istream& in);
    void gettasks(size_t t, std::istream& in);
    void solve_basic();
};
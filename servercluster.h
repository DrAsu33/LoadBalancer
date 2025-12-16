#pragma once 
#include "server.h"
#include "task.h"
#include <vector>
#include <iostream>

constexpr int INF = 0x3FFFFFFF;
// 服务器阵列的数据结构
class ServerCluster
{
private:
    std::vector<Server> servers;  //存放所有服务器的数据
    std::vector<std::vector<int>> adjacency_matrix; //邻接矩阵，-1代表不连通
    std::vector<Task> tasks;
    int server_num;
    int task_num;
    std::vector<int> tasks_of_node(int i);

public:
    ServerCluster(int n);
    void getnodes(int n, std::istream& in);
    void getedges(int m, std::istream& in);
    void gettasks(int t, std::istream& in);
    void solve_basic();
};
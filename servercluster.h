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
    std::vector<Server> servers;                    // 存放所有服务器的数据
    std::vector<std::vector<int>> adjacency_matrix; // 邻接矩阵，INF表示不连通，表示path_cost
    std::vector<std::vector<int>> bandwidth_matrix; // 带宽矩阵
    std::vector<Task> tasks;                        // 存放所有任务的数据
    std::vector<std::vector<int>> parent;           // 调用floyd算法时，存放最短路径

    void run_floyd();                               // 基础floyd算法
    void run_floyd_parent();                        // floyd算法，生成在parent矩阵中生成最短路径的节点路径
    std::vector<size_t> get_path(size_t from, size_t to);  // 生成一个节点到另一个节点的路径
 
public:
    ServerCluster(size_t n);
    void getnodes(size_t n, std::istream& in);      // 从指定的输入流中读取服务器节点信息
    void getedges(size_t m, std::istream& in);      // 从指定的输入流中读取网络链路信息
    void gettasks(size_t t, std::istream& in);      // 从指定的输入流中读取任务信息
    void solve_basic();                             // 启动优化函数， 该函数是基本要求的函数
    void solve_advanced();                          // 启动优化函数， 该函数是高级要求的函数
    void solve_additional();                        // 启动优化函数， 该函数是附加要求的函数
};
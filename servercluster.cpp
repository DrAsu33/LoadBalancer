#include "servercluster.h"

// 构造函数， 为所有vector分配内存
ServerCluster::ServerCluster(int n)
{
    server_num = n;
    servers.resize(n);
    // 为每个服务器编号（从0开始）
    // 涉及到服务器，任务编号都需要调整索引
    for(int i = 0; i < n; i++)
        servers[i].index = i;
    
    adjacency_matrix.resize(n);      
    for(int i = 0; i < n; i++)
    {
        adjacency_matrix[i].resize(n);
        for(int j = 0; j < n; j++)
            adjacency_matrix[i][j] = (i == j) ? 0 : INF;
    }
    // 初始化全部INF, 如果是对角线元素则设为0
}


// 按行读入服务器节点
void ServerCluster::getnodes(int n, std::istream& in)
{
    int node_id, capacity;
    for(int i = 0; i < n; i++)
    {
        in >> node_id >> capacity;
        servers[node_id - 1].capacity = capacity;
    }
}

// 按行读入网络链路
void ServerCluster::getedges(int m, std::istream& in)
{
    int u, v, path_cost, bandwidth;
    for(int i = 0; i < m; i++)
    {
        in >> u >> v >> path_cost >> bandwidth;
        adjacency_matrix[u-1][v-1] = path_cost;
        adjacency_matrix[v-1][u-1] = path_cost;
    }
}

// 按行读入任务
void ServerCluster::gettasks(int t, std::istream& in)
{
    int task_id, start_node, demand;
    tasks.resize(t); task_num = t;
    for(int i = 0; i < t; i++)
    {
        in >> task_id >> start_node >> demand;
        tasks[i].index = task_id;
        tasks[i].start_node = start_node;
        tasks[i].current_node = start_node;
        tasks[i].demand = demand;
        // 直接更新服务器负载和节点任务
        servers[start_node-1].gpu_used += demand;
        servers[start_node-1].assigned_tasks.push_back(i);
    }
}

// 启动优化函数，该函数是基本要求的函数
void ServerCluster::solve_basic()
{
    // 分为超载的服务器和未超载的服务器
    std::vector<int> overloaded, available;



}
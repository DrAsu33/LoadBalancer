#include "servercluster.h"

// 构造函数， 为所有vector分配内存
ServerCluster::ServerCluster(size_t n)
{
    server_num = n;
    servers.resize(n);
    // 为每个服务器编号（从0开始）
    // 涉及到服务器，任务编号都需要调整索引
    for(size_t i = 0; i < n; i++)
        servers[i].index = i;
    
    adjacency_matrix.resize(n);      
    for(size_t i = 0; i < n; i++)
    {
        adjacency_matrix[i].resize(n);
        for(size_t j = 0; j < n; j++)
            adjacency_matrix[i][j] = (i == j) ? 0 : INF;
    }
    // 初始化全部INF, 如果是对角线元素则设为0
}


// 按行读入服务器节点
void ServerCluster::getnodes(size_t n, std::istream& in)
{
    size_t node_id; int capacity;
    for(size_t i = 0; i < n; i++)
    {
        in >> node_id >> capacity;
        servers[node_id - 1].capacity = capacity;
    }
}

// 按行读入网络链路
void ServerCluster::getedges(size_t m, std::istream& in)
{
    // 确保输入的索引没有0
    size_t u, v; int path_cost, bandwidth;
    for(size_t i = 0; i < m; i++)
    {
        in >> u >> v >> path_cost >> bandwidth;
        adjacency_matrix[u-1][v-1] = path_cost;
        adjacency_matrix[v-1][u-1] = path_cost;
    }
}

// 按行读入任务
void ServerCluster::gettasks(size_t t, std::istream& in)
{
    size_t task_id, start_node; int demand;
    tasks.resize(t); task_num = t;
    for(size_t i = 0; i < t; i++)
    {
        in >> task_id >> start_node >> demand;
        tasks[i].index = task_id - 1;
        tasks[i].start_node = start_node - 1;
        tasks[i].current_node = start_node - 1;
        tasks[i].demand = demand;
        // 直接更新服务器负载和节点任务
        servers[start_node-1].gpu_used += demand;
        servers[start_node-1].assigned_tasks.push_back(i);
    }
}

void ServerCluster::run_floyd()
{
    for(size_t k = 0; k < server_num; k++)
        for(size_t i = 0; i < server_num; i++)
            for(size_t j = 0; j < server_num; j++)
                if(adjacency_matrix[i][k] != INF && adjacency_matrix[k][j] != INF)
                    adjacency_matrix[i][j] = 
                    std::min(adjacency_matrix[i][j], 
                    adjacency_matrix[i][k] + adjacency_matrix[k][j]);
}
#include "servercluster.h"

// 构造函数， 为所有vector分配内存
ServerCluster::ServerCluster(int n)
{
    servernum = n;
    servers.resize(n);
    // 为每个服务器编号（从0开始）
    // 涉及到服务器，任务编号都需要调整索引
    for(int i = 0; i < n; i++)
        servers[i].index = i;
    
    adjacencymatrix.resize(n);      
    for(int i = 0; i < n; i++)
    {
        adjacencymatrix[i].resize(n);
        for(int j = 0; j < n; j++)
            adjacencymatrix[i][j] = -1;
    }
    // 初始化全部为-1
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
        adjacencymatrix[u][v] = path_cost;
        adjacencymatrix[v][u] = path_cost;
    }
}

// 按行读入任务
void ServerCluster::gettasks(int t, std::istream& in)
{
    int task_id, start_node, demand;
    tasks.resize(t); tasknum = t;
    for(int i = 0; i < t; i++)
    {
        in >> task_id >> start_node >> demand;
        tasks[i].index = task_id;
        tasks[i].start_node = start_node;
        tasks[i].demand = demand;
        // 直接更新服务器负载
        servers[start_node].GPU_used += demand;
    }
}

// 启动优化函数，该函数是基本要求的函数
void ServerCluster::optimize_basic()
{
    // 分为超载的服务器和未超载的服务器
    std::vector<int> overloaded, available;
    // 记录每个任务最后的终点
    std::vector<int> taskdestinations;

    for(auto& server : servers)
    {
        if(server.GPU_used > server.capacity)
            overloaded.push_back(server.index);
        else if(server.GPU_used < server.capacity)
            available.push_back(server.index);
    }
}
#include "servercluster.h"

// 构造函数， 为所有vector分配内存
ServerCluster::ServerCluster(size_t n)
{
    servers.resize(n);
    // 为每个服务器编号（从0开始）
    // 涉及到服务器，任务编号都需要调整索引
    for(size_t i = 0; i < n; i++)
        servers[i].index = i;
    
    adjacency_matrix.resize(n);
    bandwidth_matrix.assign(n, std::vector<int>(n, 0));   
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
        //  保证读入的node_id合法
        if(node_id == 0 || node_id > servers.size()) continue;
        servers[node_id - 1].capacity = capacity;
    }
}

// 按行读入网络链路
void ServerCluster::getedges(size_t m, std::istream& in)
{
    size_t u, v; int path_cost, bandwidth;
    for(size_t i = 0; i < m; i++)
    {
        in >> u >> v >> path_cost >> bandwidth;
        // 确保读入的数据合法
        if(u == 0 || v == 0 || u == v || u > servers.size() || v > servers.size()) continue;
        adjacency_matrix[u-1][v-1] = path_cost;
        adjacency_matrix[v-1][u-1] = path_cost;
        bandwidth_matrix[u-1][v-1] = bandwidth;
        bandwidth_matrix[v-1][u-1] = bandwidth;
    }
}

// 按行读入任务
void ServerCluster::gettasks(size_t t, std::istream& in)
{
    size_t task_id, start_node; int demand;
    tasks.resize(t);
    for(size_t i = 0; i < t; i++)
    {
        in >> task_id >> start_node >> demand;
        // 确保读入的数据合法
        if(start_node == 0 || start_node > servers.size()) continue;
        tasks[i].index = task_id - 1;
        tasks[i].start_node = start_node - 1;
        tasks[i].current_node = start_node - 1;
        tasks[i].demand = demand;
        // 直接更新服务器负载和节点任务
        servers[start_node-1].gpu_used += demand;
        servers[start_node-1].assigned_tasks.push_back(i);
    }
}

// 经典floyd算法检验连通性
void ServerCluster::run_floyd()
{
    for(size_t k = 0; k < servers.size(); k++)
    { 
        for(size_t i = 0; i < servers.size(); i++)
        {
            if(adjacency_matrix[i][k] == INF) continue;  // 如果i和k不联通，就省略后续的计算步骤
            for(size_t j = 0; j < servers.size(); j++)
                if(adjacency_matrix[k][j] != INF)
                    adjacency_matrix[i][j] = 
                    std::min(adjacency_matrix[i][j], 
                    adjacency_matrix[i][k] + adjacency_matrix[k][j]);
        }
    }
}

// 做floyd算法的同时， 存储路径到parent数组
void ServerCluster::run_floyd_parent()
{
    // 先初始化parent数组
    size_t server_num = servers.size();
    parent.assign(server_num, std::vector<int>(server_num, -1));
    for(size_t i = 0; i < server_num; i++)
    {
        for(size_t j = 0; j < server_num; j++)
        {
            if(i == j) continue;  // 对角线应该没有路径
            if(adjacency_matrix[i][j] != INF)
                parent[i][j] = (int)j;
        }
    }
    // 再运行floyd算法
    for(size_t k = 0; k < servers.size(); k++)
    {
        for(size_t i = 0; i < servers.size(); i++)
        {
            if(adjacency_matrix[i][k] == INF) continue;
            for(size_t j = 0; j < servers.size(); j++)              
                if(adjacency_matrix[i][k] + adjacency_matrix[k][j] < adjacency_matrix[i][j])
                {
                    adjacency_matrix[i][j] = adjacency_matrix[i][k] + adjacency_matrix[k][j];
                    parent[i][j] = parent[i][k];
                }
        }
    }
}

// 生成一个节点到另一个节点的路径
std::vector<size_t> ServerCluster::get_path(size_t from, size_t to)
{
    std::vector<size_t> path;
    // 如果parent数组还没有被初始化则报错
    if(parent.empty())
    {
        std::cerr << "Vector parent is uninitialized!" << std::endl;
        return path;
    }
    // 如果不连通，直接返回空数组
    if(parent[from][to] == -1)
        return path;
    // 将每一个点加入数组
    path.push_back(from);
    while(from != to)
    {
        from = (size_t)parent[from][to];
        path.push_back(from);
    }
    return path;
}

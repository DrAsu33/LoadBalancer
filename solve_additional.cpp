#include "servercluster.h"

// 启动优化函数， 该函数是附加要求的函数
void ServerCluster::solve_additional()
{
    // 迁移计划的数据结构
    struct MigrationPlan 
    {
        size_t task_id;
        size_t from_node;
        size_t to_node;
        int demand;
        bool is_completed;
        std::vector<size_t> path_nodes;
        MigrationPlan(size_t id, size_t f, size_t t, int d, const std::vector<size_t>& p)
            : task_id(id), from_node(f), to_node(t), demand(d), is_completed(false), path_nodes(p) {}
    };
    // 先使用floyd算法检测每两个服务器是否连通, 并储存最短路径
    run_floyd_parent();

    int timestep = 0;
    std::vector<MigrationPlan> pending_queue;

    // 贪心策略把所有需要的迁移计划加入队列
    for(auto& server : servers)
    {
        // 该服务器没有超载，不需要动
        if(server.gpu_used <= server.capacity || server.assigned_tasks.empty())
            continue;

        int overflow = server.gpu_used - server.capacity;
        // 按照任务的要求负载从大到小排序 从大到小传输走任务
        std::sort(server.assigned_tasks.begin(), server.assigned_tasks.end(),
            [this](size_t task_idx_a, size_t task_idx_b) 
            {
                return this->tasks[task_idx_a].demand > this->tasks[task_idx_b].demand;
            }
        );      
        
        auto it = server.assigned_tasks.begin();
        while(it != server.assigned_tasks.end() && overflow > 0)
        {
            size_t task_idx = *it; 
            Task& task = tasks[task_idx];
            bool moved = false;
            // 用自己作为初值，如果操作完minserver还是自己说明没找到
            size_t best_dest_index = server.index; int min_cost = INF;
            // 尝试将该任务迁移到其他服务器
            for(size_t dest_idx = 0; dest_idx < servers.size(); dest_idx++)
            {
                // 不能传输给自己和不连通的服务器
                if(server.index == dest_idx || adjacency_matrix[server.index][dest_idx] == INF)
                    continue;

                Server& dest_server = servers[dest_idx];
                // 可以容纳该任务,如果路径更短则更新
                if(dest_server.capacity >= task.demand + dest_server.gpu_used)
                {
                    if(adjacency_matrix[server.index][dest_idx] < min_cost)
                    {
                        min_cost = adjacency_matrix[server.index][dest_idx];
                        best_dest_index = dest_idx;
                    }
                }
            }        
            // 把该传输计划存入队列
            if(best_dest_index != server.index)
            {
                pending_queue.push_back(MigrationPlan(task_idx, server.index, best_dest_index, task.demand, get_path(server.index, best_dest_index)));
                server.gpu_used -= task.demand;
                overflow -= task.demand;
                servers[best_dest_index].gpu_used += task.demand;

                it = server.assigned_tasks.erase(it);
                servers[best_dest_index].assigned_tasks.push_back(task_idx);
                task.current_node = best_dest_index;
                task.migration_cost += min_cost * task.demand;
                moved = true; 
            }
            if(!moved)
                it++;
        }
    }
    
    size_t count = 0; // 记录已经迁移的任务数量 
    // 一直循环到所有任务迁移结束
    while(count < pending_queue.size())
    {
        timestep++; // 时间递增， 从1开始
        auto current_bandwidth = bandwidth_matrix;
        bool deadlock_check = false; // 检测是否产生死锁，导致死循环
        for(auto& plan : pending_queue)
        {
            // 如果任务已经迁移结束则跳过
            if(plan.is_completed)
                continue;

            bool enable = true;  // 是否有足够的带宽迁移
            std::vector<size_t> path = plan.path_nodes;
            for(size_t i = 0; i < path.size() - 1; i++)
            {
                if(current_bandwidth[path[i]][path[i+1]] < plan.demand)
                    enable = false;
            }
            // 如果带宽不足就跳过
            if(!enable)
                continue;

            count++;
            // 依次消耗带宽
            for(size_t i = 0; i < path.size() - 1; i++)
                current_bandwidth[path[i]][path[i+1]] -= plan.demand;
            plan.is_completed = true;
            deadlock_check = true;
            std::cout << timestep << " " << plan.task_id + 1 << " " << plan.from_node + 1 << " " << plan.to_node + 1 << std::endl;
        }
        if (!deadlock_check && count < pending_queue.size()) 
        {
            std::cerr << "Error: Deadlock detected at timestep " << timestep << "! Bandwidth insufficient." << std::endl;
            break; 
        }
    }
    std::cout << timestep << std::endl;

    int total_cost = 0;
    // 输出每个任务的迁移结果
    for(const auto& task : tasks)
    {
        std::cout << task.index + 1 << " " << task.start_node + 1 << " " << 
        task.current_node  + 1 << " " << task.migration_cost << std::endl;
        total_cost += task.migration_cost;
    }
    // 输出每台服务器最终负载
    for(const auto& server : servers)
        std::cout << server.index + 1 << " " << server.gpu_used << std::endl;
    // 输出总共的迁移成本
    std::cout << total_cost << std::endl;
}
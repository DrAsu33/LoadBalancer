#include "servercluster.h"
#include <iostream>

// 启动优化函数，该函数是基本要求的函数
void ServerCluster::solve_basic()
{
    // 先使用floyd算法检测每两个服务器是否连通
    run_floyd();

    // 贪心策略迁移任务
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
            // 尝试将该任务迁移到其他服务器
            for(size_t dest_idx = 0; dest_idx < servers.size(); dest_idx++)
            {
                // 不能传输给自己和不连通的服务器
                if(server.index == dest_idx || adjacency_matrix[server.index][dest_idx] == INF)
                    continue;

                Server& dest_server = servers[dest_idx];
                // 可以容纳该任务,立即迁移
                if(dest_server.capacity >= task.demand + dest_server.gpu_used)
                {
                    server.gpu_used -= task.demand;
                    overflow -= task.demand;
                    dest_server.gpu_used += task.demand;

                    it = server.assigned_tasks.erase(it);
                    dest_server.assigned_tasks.push_back(task_idx);
                    task.current_node = dest_idx;
                    moved = true; break;  // 每个任务只迁移一次
                }
            }
            // 如果移动了，erase的时候迭代器就会指向下一个元素，不需要递增
            if(!moved)
                it++;
        }    
    }
    
    // 输出每个任务的迁移结果
    for(const auto& task : tasks)
        std::cout << task.index + 1 << " " << task.start_node + 1 << " " << task.current_node  + 1 << std::endl;
    // 输出每台服务器最终负载
    for(const auto& server : servers)
        std::cout << server.index + 1 << " " << server.gpu_used << std::endl;
}
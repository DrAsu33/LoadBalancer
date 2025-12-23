#pragma once

// 任务信息的数据结构
struct Task
{
    size_t index;          // 任务的序号
    size_t start_node;     // 任务起始所在的节点
    size_t current_node;   // 任务目前所在的节点
    int demand;            // 任务所需的负载
    long long migration_cost;    // 任务迁移所需要的成本
    Task(size_t i = 0, size_t s = 0, int g = 0) : 
    index(i), start_node(s), current_node(s), demand(g), migration_cost(0) {}
};
#pragma once

// 任务信息的数据结构
struct Task
{
    size_t index;
    size_t start_node;
    size_t current_node;
    int demand;
    int migration_cost;
    Task(size_t i = 0, size_t s = 0, int g = 0) : index(i), start_node(s), current_node(s), demand(g), migration_cost(0) {}
};
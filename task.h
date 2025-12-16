#pragma once

// 任务信息的数据结构
struct Task
{
    int index;
    int start_node;
    int demand;
    Task(int i = 0, int s = 0, int g = 0) : index(i), start_node(s), demand(g) {}
};
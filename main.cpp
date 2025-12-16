#include "server.h"
#include "servercluster.h"
#include "task.h"
#include <iostream>

int main()
{
    int N, M, T;
    std::cin >> N >> M >> T;
    ServerCluster servercluster(N);
    servercluster.getnodes(N, std::cin);
    servercluster.getedges(M, std::cin);
    servercluster.gettasks(T, std::cin);
    servercluster.optimize();
    return 0;
}
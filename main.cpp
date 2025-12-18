#include "server.h"
#include "servercluster.h"
#include "task.h"
#include <iostream>

int main()
{
    size_t N, M, T;
    std::cin >> N >> M >> T;
    ServerCluster servercluster(N);
    servercluster.getnodes(N, std::cin);
    servercluster.getedges(M, std::cin);
    servercluster.gettasks(T, std::cin);
    servercluster.solve_advanced();
    servercluster.print_results(std::cout);
    return 0;
}
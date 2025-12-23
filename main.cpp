#include "server.h"
#include "servercluster.h"
#include "task.h"
#include <iostream>
#include <fstream>

int main()
{
    size_t N, M, T;
    std::ifstream file("test_case.txt");
    std::istream& in = file.is_open() ? file : std::cin;
    in >> N >> M >> T;

    ServerCluster servercluster(N);
    servercluster.getnodes(N, in);
    servercluster.getedges(M, in);
    servercluster.gettasks(T, in);
    servercluster.solve_additional();
    return 0;
}
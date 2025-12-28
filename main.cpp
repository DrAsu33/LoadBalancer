#include "server.h"
#include "servercluster.h"
#include "task.h"
#include <iostream>
#include <fstream>

int main()
{
    size_t N, M, T;
    std::ifstream infile("test_case2.txt");
    std::istream& in = infile;
    in >> N >> M >> T;

    ServerCluster servercluster(N);
    servercluster.getnodes(N, in);
    servercluster.getedges(M, in);
    servercluster.gettasks(T, in);
    servercluster.solve_advanced();
    return 0;
}
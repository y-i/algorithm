#include <iostream>
#include <ratio>
#include "dinic.hpp"
using namespace std;

int main() {
    int n = 5;
    Dinic<int> mf(n);
    mf.add_edge(0, 1, 9);
    mf.add_edge(0, 2, 3);
    mf.add_edge(1, 3, 4);
    mf.add_edge(1, 2, 3);
    mf.add_edge(2, 3, 5);
    mf.add_edge(3, 4, 8);
    mf.add_edge(2, 4, 4);

    cout << mf.max_flow(0, 4) << endl;

    return 0;
}

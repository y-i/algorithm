#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include "./binary_heap.h"
#include "./binomial_heap.h"
using namespace std;

int main() {
    mt19937 mt((random_device())());
    vector<long long> a(1000000);
    for (int i = 0; i < 1000000; ++i) a[i] = i;
    shuffle(a.begin(), a.end(), mt);
    {
        time_t start = clock();
        BinaryHeap<long long> h;
        for (auto &v: a) h.push(v);
        while (!h.empty()) h.pop();
        cout << (clock() - start) / CLOCKS_PER_SEC << endl;
    }
    {
        time_t start = clock();
        BinomialHeap<long long> h;
        for (auto &v: a) h.push(v);
        while (!h.empty()) h.pop();
        cout << (clock() - start) / CLOCKS_PER_SEC << endl;
    }

    return 0;
}

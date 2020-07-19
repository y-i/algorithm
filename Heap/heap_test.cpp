#include <iostream>
#include <exception>
#include "./binary_heap.h"
#include "./binomial_heap.h"
using namespace std;

int main() {
    try {
        // BinaryHeap<long long> h;
        BinomialHeap<long long, less<long long>> h;

        for (int i = 0; i < 10; ++i) {
            h.push((i * 3) % 10);
            cout << "pushed: " << (i * 3) % 10 << endl;
            cout << "htop: " << h.top() << endl;
            cout << "---" << endl;
        }

        while (!h.empty()) {
            cout << h.top() << endl;
            h.pop();
        }
    } catch (const exception &e) {
        cerr << e.what() << endl;
        cerr << "Error" << endl;
    }

    return 0;
}

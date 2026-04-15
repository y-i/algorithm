#include "persistent_segment_tree.h"

int op(int a, int b) {
    return a + b;
}
int e() {
    return 0;
}

int main() {
    int n = 10;
    PersistentSegmentTree<int, op, e> segtree(n);
}

#include <iostream>
#include "trie.h"
#include <string>
using namespace std;

int main() {
    Trie trie;

    trie.addString("hoge");
    trie.addString("hogo");
    trie.addString("foobar");

    cout << trie.find("hogo") << endl;
    cout << trie.find("foobar") << endl;
    cout << trie.find("foo") << endl;
    cout << trie.find("fooba") << endl;

    return 0;
}

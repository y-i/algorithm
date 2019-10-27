#include <iostream>
#include <string>
#include "./modules/suffix_array.h"
using namespace std;

int main() {
    string s = "hogefugafoobarfoo";
    SuffixArray sa(s);

    cout << sa.find("hoge") << endl;
    cout << sa.find("z") << endl;
    cout << sa.find("aogefugafoobarfoo") << endl;
    cout << sa.find("fuga") << endl;
    cout << sa.find("foo") << endl;
}

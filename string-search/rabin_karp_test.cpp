#include <iostream>
#include "modules/rabin_karp.h"

using namespace std;

int main() {
    RabinKarp rk1("orangerange"), rk2("hogefugafoobar");
    cout << rk1.find("range") << endl;
    cout << rk1.find("renge") << endl;
    cout << rk2.find("fugafoo") << endl;
    cout << rk2.find("foobarfoo") << endl;

    return 0;
}

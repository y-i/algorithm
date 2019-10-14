#include <iostream>
#include "modules/boyer_moore.h"

using namespace std;

int main() {
    std::cout << (BoyerMoore::find("range", "orangerange")) << std::endl;
    std::cout << (BoyerMoore::find("renge", "orangerange")) << std::endl;
    std::cout << (BoyerMoore::find("fugafoo", "hogefugafoobar")) << std::endl;
}

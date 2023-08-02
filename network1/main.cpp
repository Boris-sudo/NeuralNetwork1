#include <iostream>
#include <cmath>
#include <fstream>
#include "myNeuro.h"


using namespace std;


int main() {
    Network a=Network();
    vector<string> s(MATRIX_SIZE);
    int result;


    while (true) {
        cin>>result;
        if (result<0||result>9) break;
        for (int i=0; i<MATRIX_SIZE; ++i) cin>>s[i];
        a.update(s, result);
    }
    for (int i=0; i<5; ++i) {
        for (int j=0; j<MATRIX_SIZE; ++j) cin>>s[j];
        a.get_result(s);
    }
}

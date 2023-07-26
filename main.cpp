#include <iostream>
#include <cmath>
#include <fstream>
#include "myNeuro.h"

using namespace std;

// изначально мы имеем матрицу 7 на 7
/*
111111
000000
000110
001100
011000
110000
*/

int main() {
    Network a = Network();
    vector<string> s(6);
    int result;


    while (true) {
        cin >> result;
        if (result == 3) break;
        for (int i = 0; i < 6; ++i) cin >> s[i];
        a.update(s, result);
    }
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 6; ++j) cin >> s[j];
        a.get_result(s);
    }
}

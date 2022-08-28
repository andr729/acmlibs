/**
 * @file pref_pref.cpp
 * @author andr729
 * @brief
 * State: +
 * Notes: 
 */

#include <vector>
#include <string>
#include <iostream>

std::vector<int> pref_pref(std::string s) {
    int l = 0;
    int r = 0;
    int n = s.size();
    std::vector<int> out(n, 0);

    int counter = n;
    for (int i = 1; i < n; i++) {
        if (i < r) {
            out[i] = std::min(out[i - l], r - i);
        }
        while (i + out[i] < n and s[out[i]] == s[i + out[i]]) {
            counter++;
            out[i]++;
        }
        if (out[i] + i > r) {
            l = i;
            r = i + out[i];
        }
    }
    out[0] = n;
    return out;
}

int main() {
    for (auto i: pref_pref("asdbasalasdiwa")) {
        std::cout << i << " ";
    }
    std::cout << "\n";
}
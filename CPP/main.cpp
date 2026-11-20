//
// Created by User on 11/20/2026.
//
//
#include <iostream>
#include <vector>
#include <cstdio>
int main() {
    std::vector<int> numbers;
    int n;
    std::cin >> n;
    numbers.resize(n+1);
    for (int i = 1; i < n+1; i++) {
        numbers[i]=i;
    }
    while (numbers.size()>2) {
        // step 1: keep odd positions
        int w = 1;
        for (int i = 1; i < numbers.size(); i++) {
            if (i%2!=0) numbers[w++] = numbers[i];
        }
        numbers.erase(numbers.begin()+w, numbers.end());
        if (numbers.size()<=2) break;

        // step 2: keep even positions
        w = 1;
        for (int i = 1; i < numbers.size(); i++) {
            if (i%2==0) numbers[w++] = numbers[i];
        }
        numbers.erase(numbers.begin()+w, numbers.end());
        if (numbers.size()<=2) break;

        // step 3: keep positions not divisible by 3
        w = 1;
        for (int i = 1; i < numbers.size(); i++) {
            if (i%3!=0) numbers[w++] = numbers[i];
        }
        numbers.erase(numbers.begin()+w, numbers.end());
    }
    printf("%d\n", numbers[1]);
}

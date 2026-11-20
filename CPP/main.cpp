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
        //get rid of even indices.
        //With w, we keep track of odd indices.
        //Only increase w when i is odd.
        int w = 1;
        for (int i = 1; i < numbers.size(); i++) {
            if (i%2!=0) numbers[w++] = numbers[i];
        }
        numbers.erase(numbers.begin()+w, numbers.end());
        if (numbers.size()<=2) break;

        //remove odd indices.
        w = 1;
        for (int i = 1; i < numbers.size(); i++) {
            if (i%2==0) numbers[w++] = numbers[i];
        }
        numbers.erase(numbers.begin()+w, numbers.end());
        if (numbers.size()<=2) break;

        //remove indices divisible by 3
        w = 1;
        for (int i = 1; i < numbers.size(); i++) {
            if (i%3!=0) numbers[w++] = numbers[i];
        }
        numbers.erase(numbers.begin()+w, numbers.end());
    }
    printf("%d\n", numbers[1]);
}

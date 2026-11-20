//
// Created by User on 11/20/2026.
//
//
#include <iostream>
#include <vector>
int main() {
    std::vector<int> numbers;
    int n;
    std::cin >> n;
    numbers.resize(n+1);
    for (int i = 1; i < n+1; i++) {
        numbers[i]=i;
    }
    while (numbers.size()>1) {
        for (int i = n+1; i >0; i--) {
            if (i%2==0) {
                numbers.erase(numbers.begin()+i);
            }
        }
        for (int i = n+1; i >0; i--) {
            if (i%2!=0) {
                numbers.erase(numbers.begin()+i);
            }
        }
        for (int i = n+1; i >0; i--) {
            if (i%3==0) {
                numbers.erase(numbers.begin()+i);
            }

        }
    }
    printf("%d\n", numbers[1]);
}

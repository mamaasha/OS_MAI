#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <dlfcn.h>

int main () {
    void* handle = NULL;
    int (*GCF)(int A, int B);
    float (*Square)(float A, float B);
    const char* lib_array[] = {"libd1.so", "libd2.so"};
    int curlib;
    int start_library;
    std::cout << "Enter start library: " << std::endl;
    std::cout << '\t' << "1 for using first library" << std::endl;
    std::cout << '\t' << "2 for using second library" << std::endl;
    std::cin >> start_library;
    bool flag = true;
    while (flag) {
        if (start_library == 1) {
            curlib = 0;
            flag = false;
        }
        else if (start_library == 2) {
            curlib = 1;
            flag = false;
        }
        else {
            std::cout << "You should enter only 1 or 2!" << std::endl;
            std::cin >> start_library;
        }
    }
    handle = dlopen(lib_array[curlib], RTLD_LAZY);
    if (!handle) {
        std::cout << "An error while opening library has been detected" << std::endl;
        exit(EXIT_FAILURE);
    }
    GCF = (int(*)(int, int))(dlsym(handle, "GCF"));
    Square = (float(*)(float, float))(dlsym(handle, "Square"));
    int command;
    std::cout << "Hello there! Please enter your command according to next rules: " << std::endl;
    std::cout << '\t' << "0 for changing the contract;" << std::endl;
    std::cout << '\t' << "1 for calculating the GCD; " << std::endl;
    std::cout << '\t' << "2 for calculating the square; " << std::endl; 
    while (printf("Please enter your command: ") && (scanf("%d", &command)) != EOF) {
        if (command == 0) {
            dlclose(handle);
            if (curlib == 0) {
                curlib = 1 - curlib;
                handle = dlopen(lib_array[curlib], RTLD_LAZY);
                if (!handle) {
                    std::cout << "An error while opening library has been detected" << std::endl;
                    exit(EXIT_FAILURE);
                }
                GCF = (int(*)(int, int))(dlsym(handle, "GCF"));
                Square = (float(*)(float, float))(dlsym(handle, "Square")); 
            }
            else if (curlib == 1) {
                curlib = 1 - curlib;
                handle = dlopen(lib_array[curlib], RTLD_LAZY);
                if (!handle) {
                    std::cout << "An error while opening library has been detected" << std::endl;
                    exit(EXIT_FAILURE);
                }
                GCF = (int(*)(int, int))(dlsym(handle, "GCF"));
                Square = (float(*)(float, float))(dlsym(handle, "Square"));
            }
            std::cout << "You have changed contracts!" << std::endl;
        }
        else if (command == 1) {
            int A, B;
            std::cout << "Please enter two natural numbers: ";
            std::cin >> A >> B;
            int gcd = GCF(A, B);
            if (gcd == -1) {
                std::cout << "Please enter positive variables!" << std::endl;
            }
            else {
                std::cout << "The greatest common divisor is: " << gcd << std::endl;
            }
        }
        else if (command == 2) {
            float A, B, square;
            std::cout << "Please enter sides of your figure: ";
            std::cin >> A >> B;
            square = Square(A, B);
            if (square == -1) {
                std::cout << "Please enter positive variables!" << std::endl;
            }
            else {
                std::cout << "The square of your figure is: " << square << std::endl;
            }
        }

        else {
            std::cout << "Invalid command! Please try again." << std::endl;
        }
    }
    return 0;
}

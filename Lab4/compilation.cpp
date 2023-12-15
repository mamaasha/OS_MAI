#include <iostream>
extern "C" int GCF (int A, int B);
extern "C" float Square (float A, float B);

int main () {
    int command;
    while (printf("Please enter your command: ") && (scanf("%d", &command)) != EOF) {
        switch (command) {
            case 1: {
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
                break;
            }
            case 2: {
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
                break;
            }
            default:
                std::cout << "You must enter 1 or 2!" << std::endl;
                break;
        }
    }
    return 0;
}

extern "C" int GCF(int A, int B);
extern "C" float Square (float A, float B);


int GCF(int A, int B) {
    // Проверяем, чтобы числа были натуральными
    if (A <= 0 || B <= 0) {
        return -1;
    }
    
    // Наивный алгоритм
    int minNumber = (A < B) ? A : B;
    int gcd = 1;
    
    for (int i = 1; i <= minNumber; ++i) {
        if (A % i == 0 && B % i == 0) {
            gcd = i;
        }
    }
    
    // Возвращаем наибольший общий делитель
    return gcd;
}

float Square (float A, float B) {
    if (A <= 0 || B <= 0) {
        return -1;
    }
    float square = 0.5 * A * B;
    return square;
}
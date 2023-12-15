extern "C" int GCF(int A, int B);
extern "C" float Square (float A, float B);

int GCF(int A, int B) {
    // Проверяем, чтобы числа были натуральными
    if (A <= 0 || B <= 0) {
        return -1;
    }
    
    // Алгоритм Евклида
    while (A != B) {
        if (A > B) {
            A -= B;
        } else {
            B -= A;
        }
    }
    
    // Возвращаем наибольший общий делитель
    return A;
}

float Square (float A, float B) {
    if (A <= 0 || B <= 0) {
        return -1;
    }
    float square = A * B;
    return square;
}
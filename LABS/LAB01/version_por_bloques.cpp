#include <iostream>
#include <chrono>
using namespace std;

const int MAX = 2000;

double A[MAX][MAX], B[MAX][MAX], C[MAX][MAX];

int main()
{
    int Bloques = 64;  // Cambiar para optimización
    // Inicializa A y x, asigna y = 0
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            A[i][j] = B[i][j] = 1.0;
            C[i][j] = 0.0;
        }
    }

    auto start_time = std::chrono::high_resolution_clock::now();

    for (int I = 0; I < MAX; I += Bloques)
    {
        for (int J = 0; J < MAX; J += Bloques)
        {
            for (int K = 0; K < MAX; K += Bloques)
            {
                for (int i = I; i < min(I + Bloques, MAX); ++i)
                {
                    for (int j = J; j < min(J + Bloques, MAX); ++j)
                    {
                        int sum = 0;
                        for (int k = K; k < min(K + Bloques, MAX); ++k)
                        {
                            sum += A[i][k] * B[k][j];
                        }
                        C[i][j] += sum;
                    }
                }
            }
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    std::cout << "Tiempo primer bucle: " << duration.count() << " microsegundos" << std::endl;

    return 0;
}

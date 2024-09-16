#include <iostream>
#include <chrono>
using namespace std;

const int MAX = 2000;

double A[MAX][MAX], B[MAX][MAX], C[MAX][MAX];

int main() {
    // Inicializa A y x, asigna y = 0
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            A[i][j] = B[i][j] = 1.0;
            C[i][j] = 0.0;
        }
    }

    //tiempo
    auto start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < MAX; ++i) {
        for (int j = 0; j < MAX; ++j) {
            for (int k = 0; k < MAX; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    
    std::cout << "Tiempo primer bucle: " << duration.count() << " microsegundos" << std::endl;

    return 0;
}

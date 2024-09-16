/*
El primer bucle tiene menor tiempo de ejecucion debido a que accede a los datos
en la matriz bidimensionalmente en bloques contiguos, lo que no ocurre con el segundo buble
debido a que lo recorremos en forma de columnas y filas, ocurriendo lo contrario.
*/
/*
// 100
Tiempo primer bucle: 68 microsegundos
Tiempo segundo bucle: 92 microsegundos
// 500
Tiempo primer bucle: 1360 microsegundos
Tiempo segundo bucle: 3033 microsegundos
// 1000
Tiempo primer bucle: 3704 microsegundos
Tiempo segundo bucle: 9218 microsegundos
// 10000 
Tiempo para el primer par de bucles: 425059 microsegundos
Tiempo para el segundo par de bucles: 2170458 microsegundos
*/

#include <iostream>
#include <chrono>
using namespace std;

const int MAX = 1000;

double A[MAX][MAX], x[MAX], y[MAX];

int main() {
    // Inicializa A y x, asigna y = 0
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            A[i][j] = 0.0;
        }
        x[i] = 0.0;
        y[i] = 0.0;
    }

    //tiempo
    auto start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            y[i] += A[i][j] * x[j];
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    
    std::cout << "Tiempo primer bucle: " << duration.count() << " microsegundos" << std::endl;

    // Asigna y = 0
    for (int i = 0; i < MAX; i++) {
        y[i] = 0.0;
    }

    // Tiempo
    start_time = std::chrono::high_resolution_clock::now();

    for (int j = 0; j < MAX; j++) {
        for (int i = 0; i < MAX; i++) {
            y[i] += A[i][j] * x[j];
        }
    }

    end_time = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

    std::cout << "Tiempo segundo bucle: " << duration.count() << " microsegundos" << std::endl;

    return 0;
}

# Laboratorio 1: Pruebas sobre el comportamiento de la memoria caché

## Universidad Nacional de San Agustín  
**Facultad de Ingeniería y Producción de Servicios**  
**Escuela Profesional de Ciencia de la Computación**  

### Curso: Computación Paralela y Distribuida  
**Autor: Manuel Ángel Nifla Llallacachi**  
**Correo: mnifla@unsa.edu.pe**

---

## Resumen

Este informe presenta la implementación, resultados y análisis de la ejecución de diversos problemas para el Laboratorio N° 1 del curso de Computación Paralela y Distribuida.

---

## Ejercicio 1: Comparación de bucles anidados

### Implementación

Se implementan dos bucles anidados que realizan la multiplicación de una matriz \( A \) por un vector \( x \) para obtener un nuevo vector \( y \).

```cpp
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

    // Primer bucle
    auto start_time = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            y[i] += A[i][j] * x[j];
        }
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    cout << "Tiempo primer bucle: " << duration.count() << " microsegundos" << endl;

    // Segundo bucle
    for (int i = 0; i < MAX; i++) y[i] = 0.0;
    start_time = std::chrono::high_resolution_clock::now();
    for (int j = 0; j < MAX; j++) {
        for (int i = 0; i < MAX; i++) {
            y[i] += A[i][j] * x[j];
        }
    }
    end_time = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    cout << "Tiempo segundo bucle: " << duration.count() << " microsegundos" << endl;

    return 0;
}
```

### Resultados

| Datos | Tiempo Primer Bucle | Tiempo Segundo Bucle |
|-------|---------------------|----------------------|
| 100   | 68 microsegundos     | 92 microsegundos     |
| 500   | 1360 microsegundos   | 3033 microsegundos   |
| 1000  | 3704 microsegundos   | 9218 microsegundos   |
| 10000 | 425059 microsegundos | 2170458 microsegundos|

### Análisis

El primer bucle tiene un mejor rendimiento porque accede a bloques contiguos de la memoria, lo que mejora la eficiencia en caché. En cambio, el segundo bucle recorre las matrices por columnas, lo que genera más fallos de caché y aumenta el tiempo de ejecución.

---

## Ejercicio 2: Multiplicación clásica de matrices (tres bucles anidados)

### Implementación

La multiplicación de matrices clásica se implementa con tres bucles anidados para multiplicar dos matrices \( A \) y \( B \), produciendo una matriz resultante \( C \).

```cpp
#include <iostream>
#include <chrono>
using namespace std;

const int MAX = 2000;
double A[MAX][MAX], B[MAX][MAX], C[MAX][MAX];

int main() {
    // Inicializa matrices A y B
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            A[i][j] = B[i][j] = 1.0;
            C[i][j] = 0.0;
        }
    }

    // Tiempo
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
    cout << "Tiempo primer bucle: " << duration.count() << " microsegundos" << endl;

    return 0;
}
```

### Resultados

| Datos | Tiempo Primer Bucle |
|-------|---------------------|
| 100   | 4995 microsegundos   |
| 500   | 438969 microsegundos |
| 1000  | 4129539 microsegundos|
| 2000  | 50934419 microsegundos|

### Análisis

Este algoritmo tiene una complejidad de \( O(n^3) \), y no está optimizado para la jerarquía de memoria caché, lo que lo hace ineficiente para matrices grandes sin optimizaciones.

---

## Ejercicio 3: Multiplicación de matrices por bloques

### Implementación

La multiplicación por bloques optimiza el acceso a la memoria dividiendo las matrices en submatrices más pequeñas que caben mejor en caché, lo que reduce la cantidad de accesos a la memoria principal.

```cpp
#include <iostream>
#include <chrono>
using namespace std;

const int MAX = 2000;
double A[MAX][MAX], B[MAX][MAX], C[MAX][MAX];

int main() {
    int Bloques = 64; // Tamaño de bloque
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            A[i][j] = B[i][j] = 1.0;
            C[i][j] = 0.0;
        }
    }

    auto start_time = std::chrono::high_resolution_clock::now();
    for (int I = 0; I < MAX; I += Bloques) {
        for (int J = 0; J < MAX; J += Bloques) {
            for (int K = 0; K < MAX; K += Bloques) {
                for (int i = I; i < min(I + Bloques, MAX); ++i) {
                    for (int j = J; j < min(J + Bloques, MAX); ++j) {
                        int sum = 0;
                        for (int k = K; k < min(K + Bloques, MAX); ++k) {
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
    cout << "Tiempo primer bucle: " << duration.count() << " microsegundos" << endl;

    return 0;
}
```

### Resultados

| Datos | Tiempo Primer Bucle |
|-------|---------------------|
| 100   | 6997 microsegundos   |
| 500   | 625204 microsegundos |
| 1000  | 4975500 microsegundos|
| 2000  | 37875690 microsegundos|

### Análisis

Este algoritmo mejora el rendimiento al utilizar submatrices, lo que optimiza el uso de la memoria caché. En sistemas con jerarquía de memoria, esta implementación es más eficiente que la multiplicación de matrices clásica.

---

## Referencias

[1] P. S. Pacheco, *An Introduction to Parallel Programming*. Burlington, MA, USA: Morgan Kaufmann, 2011.

**Repositorio del código**: [Repositorio de GitHub](https://github.com/AngelNifla/EPCC-COMPUTACION-PARALELA-Y-DISTRIBUIDA-2024/tree/master/LABS/LAB01)

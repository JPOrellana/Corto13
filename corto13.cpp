#include <iostream>
#include <vector>
#include <numeric> 
#include <omp.h>   

// Función principal para calcular la suma de una lista en paralelo
long long fork_join_suma(const std::vector<int>& lista, int num_hilos) {
    long long suma_total = 0;
    int tamanio_sublista = (lista.size() + num_hilos - 1) / num_hilos; // Tamaño de sublistas
    std::vector<long long> sumas_parciales(num_hilos, 0);

    std::cout << "-------------------------------------------------" << std::endl; 
    std::cout << "|        Sumatoria Parcial de cada Hilo         |" << std::endl; 
    std::cout << "-------------------------------------------------" << std::endl;

    // Paralelizar el cálculo de la suma usando OpenMP
    #pragma omp parallel num_threads(num_hilos)
    {
        int id_hilo = omp_get_thread_num();
        int inicio = id_hilo * tamanio_sublista;
        int fin = std::min((id_hilo + 1) * tamanio_sublista, (int)lista.size());

        // Sumar la sublista asignada a este hilo
        for (int i = inicio; i < fin; ++i) {
            sumas_parciales[id_hilo] += lista[i];
        }

        // Imprimir la suma parcial de cada hilo
        #pragma omp critical
        {
            std::cout << "Hilo " << id_hilo << ": Suma del rango (" << inicio << " a " << fin - 1
                      << ") = " << sumas_parciales[id_hilo] << std::endl;
        }
    }

    // Sumar todas las sumas parciales
    suma_total = std::accumulate(sumas_parciales.begin(), sumas_parciales.end(), 0LL);
    return suma_total;
}

int main() {
    // Crear una lista de números
    int num_elementos = 1000; // 1000 elementos
    std::vector<int> lista(num_elementos);
    
    // Rellenar la lista con valores del 1 al num_elementos
    std::iota(lista.begin(), lista.end(), 1);

    int num_hilos = 4; // Usar al menos 4 hilos

    // Calcular la suma en paralelo utilizando OpenMP
    long long suma_total_paralela = fork_join_suma(lista, num_hilos);

    std::cout << "-------------------------------------------------" << std::endl; 
    std::cout << "|        Sumatoria Paralela y Secuencial        |" << std::endl; 
    std::cout << "-------------------------------------------------" << std::endl; 

    // Mostrar la suma total calculada en paralelo
    std::cout << "Suma total calculada en paralelo: " << suma_total_paralela << std::endl;

    // Calcular la suma secuencialmente para comparar
    long long suma_total_secuencial = std::accumulate(lista.begin(), lista.end(), 0LL);
    std::cout << "Suma total calculada secuencialmente: " << suma_total_secuencial << std::endl;

    return 0;
}

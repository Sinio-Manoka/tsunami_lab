#include <iostream>
#include <chrono>

// Funktion 1
double expression1(double x, double z) {
    return (x / (z * z));
}

// Funktion 2
double expression2(double x, double z) {
    return x * (1 / (z * z));
}

// Benchmark-Funktion
void runBenchmark(double x, double z, int iterations) {
    auto start1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        expression1(x+i, z);
    }
    auto end1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration1 = end1 - start1;

    auto start2 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        expression2(x+i, z);
    }
    auto end2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration2 = end2 - start2;

    std::cout << "Expression 1: (x / (z * z))" << duration1.count() << " seconds\n";
    std::cout << "Expression 2: x * (1 / (z * z))" << duration2.count() << " seconds\n";
}

int main() {
    double x = 10.0, z = 6.0;
    int iterations = 1000000;

    runBenchmark(x, z, iterations);

    return 0;

    /*Ergebnis: Expression 1: (x / (z * z))0.198937 seconds
                Expression 2: x * (1 / (z * z))0.207136 seconds
                -> Expression 1 ist schneller -> ist also egal ob die Division anders ausgeführt wird
    */
}
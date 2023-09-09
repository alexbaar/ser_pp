#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <cstdlib>

using namespace std;
using namespace std::chrono;

int main() {
    // sample size; 2D matrix of size NxN -> rows x columns
    const int N = 100; 

    // vector of vectors = 2D matrice
    vector<vector<int>> A(N, vector<int>(N));
    vector<vector<int>> B(N, vector<int>(N));
    vector<vector<int>> C(N, vector<int>(N, 0));


    // Populate matrices with random values
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            A[i][j] = rand() % 100;
            B[i][j] = rand() % 100;
        }
    }

    // multiply the matrices
    // start timer; we only measure multiplication process, not creation & population of matrices
    auto start = high_resolution_clock::now();
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            for (int k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    // end timer
    auto end = high_resolution_clock::now();
    // calculate difference in miliseconds
    auto duration = duration_cast<milliseconds>(end - start);

    // save results to file
    ofstream outputFile("results.txt");
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            outputFile << " "<< C[i][j] << " ";
        }
        outputFile << endl;
    }
    
    outputFile.close();

    cout << "Sequential execution time: " << duration.count() << " ms" << endl;

    return 0;
}
#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <random>
#include <omp.h> // Include OpenMP header

using namespace std;
using namespace std::chrono;

const int N = 1000;
#define NUM_THREADS 4

struct MultiplTask {
    vector<vector<int>> &A;
    vector<vector<int>> &B;
    vector<vector<int>> &C;
    int start;
    int end;
};

// Matrix Multiply Thread Method
void matrixMultiply(MultiplTask &task) {
    // Perform matrix multiplication for the assigned range
    for (int i = task.start; i < task.end; ++i) {
        for (int j = 0; j < N; ++j) {
            for (int k = 0; k < N; ++k) {
                task.C[i][j] += task.A[i][k] * task.B[k][j];
            }
        }
    }
}

int main() {
    // Create matrices A, B, and C
    vector<vector<int>> A(N, vector<int>(N));
    vector<vector<int>> B(N, vector<int>(N));
    vector<vector<int>> C(N, vector<int>(N, 0));

    // Random Number Generation
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(0, 9);

    // Initialize matrices A and B with random values
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = dist(gen);
            B[i][j] = dist(gen);
        }
    }

    int partition_size = N / NUM_THREADS;

    auto start = high_resolution_clock::now();

    // Parallel region using OpenMP
    #pragma omp parallel num_threads(NUM_THREADS)
    {
        int thread_id = omp_get_thread_num();
        MultiplTask task{A, B, C, thread_id * partition_size, (thread_id + 1) == NUM_THREADS ? N : (thread_id + 1) * partition_size};
        matrixMultiply(task);
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);

    // File Output
    ofstream outputFile("results.txt", ios::binary);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            outputFile << " " << C[i][j] << " ";
        }
        outputFile << endl;
    }
    
    outputFile.close();

    // Print execution time
    cout << "Parallel execution time: " << duration.count() << " ms" << endl;

    return 0;
}
